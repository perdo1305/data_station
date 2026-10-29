"""Local Tk admin controls for the DBC simulation stack and current ROS topics."""
import copy
import json
import os
from pathlib import Path
import queue
import re
import signal
import subprocess
import threading
import time

from .admin_controls import (ControlConfig, control_value, discrete_values,
                             encodable_range, scenario_values, speed_to_erpm)
from .simulation_config import config_path, load_config, save_config
from .fault_tests import FAULT_PRESETS, FAULT_BUSES


def ros_name(raw):
    slug = re.sub(r'[^a-zA-Z0-9_]', '_', raw).strip('_').lower()
    return 'val_' + slug if slug and slug[0].isdigit() else slug


def updated_message(message, field, value):
    changed = copy.deepcopy(message)
    current = getattr(changed, field)
    setattr(changed, field, type(current)(value))
    return changed


def simulator_names(nodes):
    names = ['/' + '/'.join((namespace.strip('/'), name)).strip('/')
             for name, namespace in nodes if name == 'can_simulator' or name.startswith('can_simulator_')]
    if len(set(names)) != len(names):
        raise ValueError('Duplicate simulator nodes: stop the extra simulation stack first')
    return sorted(names)


class Backend:
    """One ROS worker owns clients, subscriptions, processes and temporary tests."""
    def __init__(self):
        self.commands = queue.Queue()
        self.events = queue.Queue()
        self.profiles = {}
        self.latest = {}
        self.subscriptions = {}
        self.publishers = {}
        self.stack = None
        self.recorder = None
        self.record_path = ''
        self.session = None
        self.closing = False
        self.config_file = config_path()
        self.thread = threading.Thread(target=self.run, daemon=True)
        self.thread.start()

    def submit(self, action, *args):
        self.commands.put((action, args))

    def event(self, kind, value):
        self.events.put((kind, value))

    def run(self):
        import rclpy
        from rclpy.node import Node
        import can
        import cantools
        from rclpy.qos import qos_profile_sensor_data
        from std_msgs.msg import Int32
        self.rclpy = rclpy
        self.qos = qos_profile_sensor_data
        try:
            os.environ.setdefault('ROS_DOMAIN_ID', '42')
            rclpy.init()
            self.node = Node('can_admin_panel')
            topic = os.environ.get('LART_ROS2_SET_SCREEN_TOPIC', '/dashboard/set_screen')
            self.screen_pub = self.node.create_publisher(Int32, topic, 10)
            try:
                self.start_stack()
            except Exception as exc:
                self.event('status', f'Simulation startup: {exc}')
            last_discover = -10
            last_values = 0
            while not self.closing:
                rclpy.spin_once(self.node, timeout_sec=.01)
                try:
                    action, args = self.commands.get_nowait()
                except queue.Empty:
                    action = None
                if action:
                    try:
                        getattr(self, action)(*args)
                    except Exception as exc:
                        self.event('status', f'Error: {exc}')
                now = time.monotonic()
                if now - last_discover > 3 and not self.session:
                    try:
                        self.discover()
                    except Exception as exc:
                        self.event('status', f'Discovery: {exc}')
                    last_discover = now
                try:
                    self.tick_session(now)
                except Exception as exc:
                    self.event('status', f'Test failed: {exc}')
                    self.end_test()
                if now - last_values > .25:
                    for attribute, label in (('stack', 'Simulation launch'), ('recorder', 'Bag recorder')):
                        process = getattr(self, attribute)
                        if process is not None and process.poll() is not None:
                            self.event('status', f'{label} exited with code {process.returncode}; see terminal output')
                            setattr(self, attribute, None)
                    values = {}
                    for name, profile in self.profiles.items():
                        bus = profile.get('bus')
                        if bus:
                            # ponytail: cap each pass; the next poll drains remaining frames.
                            for _ in range(300):
                                try:
                                    frame = bus.recv(timeout=0)
                                except (can.CanError, OSError) as exc:
                                    self.event('status', f'CAN monitor disconnected on {name}: {exc}')
                                    bus.shutdown()
                                    profile['bus'] = None
                                    break
                                if frame is None:
                                    break
                                try:
                                    message = profile['messages_by_id'][(frame.arbitration_id, frame.is_extended_id)]
                                    decoded = message.decode(frame.data, decode_choices=False)
                                    for key in list(profile['values']):
                                        if key[0] == message.name:
                                            del profile['values'][key]
                                    profile['values'].update({(message.name, s): v for s, v in decoded.items()})
                                except (KeyError, ValueError, cantools.database.errors.DecodeError):
                                    pass
                        if self.session and self.session['kind'] == 'direct':
                            for message_name, (topic, _) in profile['topics'].items():
                                if topic in self.latest:
                                    for sig in profile['db'].get_message_by_name(message_name).signals:
                                        field = ros_name(sig.name)
                                        if hasattr(self.latest[topic], field):
                                            profile['values'][(message_name, sig.name)] = getattr(self.latest[topic], field)
                        values[name] = dict(profile['values'])
                    self.event('values', values)
                    self.event('activity', (bool(self.session), self.stack is not None and self.stack.poll() is None,
                                            self.recorder is not None and self.recorder.poll() is None, self.record_path))
                    last_values = now
        except Exception as exc:
            self.event('status', f'ROS worker failed: {exc}. Source ROS and install/setup.bash before launching.')
        finally:
            cleanup_errors = []
            if hasattr(self, 'node'):
                self.end_test()
            for process, sig in ((self.recorder, signal.SIGINT), (self.stack, signal.SIGTERM)):
                try:
                    self.stop_process(process, sig)
                except Exception as exc:
                    cleanup_errors.append(str(exc))
                    self.event('status', f'Cleanup: {exc}')
            for profile in self.profiles.values():
                if profile.get('bus'):
                    profile['bus'].shutdown()
            if hasattr(self, 'node'):
                self.node.destroy_node()
                rclpy.try_shutdown()
            self.event('closed', cleanup_errors)

    def wait(self, future, timeout=3):
        deadline = time.monotonic() + timeout
        while not future.done() and time.monotonic() < deadline:
            self.rclpy.spin_once(self.node, timeout_sec=.02)
        if not future.done():
            future.cancel()
            raise TimeoutError('ROS parameter request timed out; check the simulator and ROS domain')
        return future.result()

    def client(self, name):
        from rclpy.parameter_client import AsyncParameterClient
        return AsyncParameterClient(self.node, name)

    def read(self, name, fields):
        from rclpy.parameter import Parameter
        from rcl_interfaces.msg import Parameter as ParameterMsg
        client = self.profiles[name]['client'] if name in self.profiles else self.client(name)
        response = self.wait(client.get_parameters(fields))
        return {key: Parameter.from_parameter_msg(ParameterMsg(name=key, value=value)).value for key, value in zip(fields, response.values)}

    def set(self, name, **values):
        from rclpy.parameter import Parameter
        simulator_names(self.node.get_node_names_and_namespaces())
        client = self.profiles[name]['client']
        result = self.wait(client.set_parameters_atomically([Parameter(key, value=value) for key, value in values.items()])).result
        if not result.successful:
            raise ValueError(result.reason)
        self.profiles[name]['params'].update(values)
        self.event('params', {n: dict(p['params']) for n, p in self.profiles.items()})

    def discover(self):
        import can
        import cantools
        from lart_bringup.topic_names import dbc_topic_prefix, lart_msg_type_names
        from rosidl_runtime_py.utilities import get_message
        names = simulator_names(self.node.get_node_names_and_namespaces())
        fields = ['dbc_path', 'can_interface', 'enabled', 'publish_hz', 'signal_controls', 'message_intervals_ms']
        changed = False
        restore_errors = []
        for removed in set(self.profiles) - set(names):
            profile = self.profiles.pop(removed)
            if profile.get('bus'):
                profile['bus'].shutdown()
            changed = True
        for name in names:
            params = self.read(name, fields)
            if not isinstance(params.get('signal_controls'), str):
                raise ValueError(f'{name} needs rebuilding with the admin controls: colcon build --packages-select sim')
            if name in self.profiles:
                self.profiles[name]['params'] = params
                continue
            path = Path(params['dbc_path'])
            files = sorted(path.glob('*.dbc')) if path.is_dir() else [path]
            db = cantools.database.Database()
            for file in files:
                db.add_dbc_file(file)
            bus = None
            try:
                bus = can.Bus(interface='socketcan', channel=params['can_interface'])
            except Exception as exc:
                self.event('status', f'CAN monitor unavailable on {params["can_interface"]}: {exc}')
            profile = {'params': params, 'db': db, 'client': self.client(name), 'bus': bus,
                       'values': {}, 'topics': {}, 'validator': ControlConfig(db.messages),
                       'messages_by_id': {(m.frame_id, m.is_extended_frame): m for m in db.messages}}
            for message in db.messages:
                topic_prefix = dbc_topic_prefix(str(path))
                topic = topic_prefix + '/' + ros_name(message.name)
                try:
                    msg_type = None
                    for type_name in lart_msg_type_names(topic_prefix, ros_name(message.name)):
                        try:
                            msg_type = get_message('lart_msgs/msg/' + type_name)
                            break
                        except (ImportError, AttributeError, ModuleNotFoundError):
                            continue
                    if msg_type is None:
                        raise AttributeError(message.name)
                    profile['topics'][message.name] = (topic, msg_type)
                    if topic not in self.subscriptions:
                        self.subscriptions[topic] = self.node.create_subscription(
                            msg_type, topic, lambda msg, t=topic: self.latest.__setitem__(t, msg), self.qos)
                except (ImportError, AttributeError, ModuleNotFoundError):
                    pass
            self.profiles[name] = profile
            try:
                self.load_settings(name)
            except (ValueError, OSError, KeyError) as exc:
                restore_errors.append(f'{name}: {exc}')
            changed = True
        if changed:
            self.event('catalog', {n: {'db': p['db'], 'params': dict(p['params']),
                                     'topics': dict(p['topics'])} for n, p in self.profiles.items()})
            self.event('status', f'Connected to {len(self.profiles)} simulator(s), ROS domain {os.environ["ROS_DOMAIN_ID"]}')
            if restore_errors:
                self.event('status', 'Saved settings were not restored: ' + '; '.join(restore_errors))
        self.event('params', {n: dict(p['params']) for n, p in self.profiles.items()})

    def idle_required(self):
        if self.session:
            raise ValueError('End or cancel the active test before editing simulation controls')

    def save_settings(self, name=None):
        self.idle_required()
        data = load_config(self.config_file)
        for target in ([name] if name else list(self.profiles)):
            params = self.read(target, ['signal_controls', 'message_intervals_ms', 'publish_hz'])
            self.profiles[target]['validator'].validate(params['signal_controls'], params['message_intervals_ms'], params['publish_hz'])
            data.setdefault('simulators', {})[target] = {
                'dbc_file': Path(self.profiles[target]['params']['dbc_path']).name,
                'signal_controls': json.loads(params['signal_controls']),
                'message_intervals_ms': json.loads(params['message_intervals_ms']),
                'publish_hz': params['publish_hz'],
            }
        save_config(data, self.config_file)
        self.event('status', f'Settings saved to {self.config_file}')

    def load_settings(self, name=None):
        self.idle_required()
        data = load_config(self.config_file)
        updates = {}
        for target in ([name] if name else list(self.profiles)):
            saved = data.get('simulators', {}).get(target)
            if saved is None:
                continue
            if saved['dbc_file'] != Path(self.profiles[target]['params']['dbc_path']).name:
                raise ValueError(f'{target}: saved DBC does not match the current simulator')
            controls, intervals = json.dumps(saved['signal_controls']), json.dumps(saved['message_intervals_ms'])
            self.profiles[target]['validator'].validate(controls, intervals, saved['publish_hz'])
            updates[target] = dict(signal_controls=controls, message_intervals_ms=intervals, publish_hz=float(saved['publish_hz']))
        for target, values in updates.items():
            self.set(target, **values)
        if name is None:
            self.event('status', f'Loaded saved controls from {self.config_file}; restart simulators for edited driving defaults')

    def start_stack(self):
        self.idle_required()
        if simulator_names(self.node.get_node_names_and_namespaces()):
            self.event('status', 'Attached to the existing simulator stack')
            self.discover()
            return
        if self.stack and self.stack.poll() is None:
            return
        self.stack = subprocess.Popen(['ros2', 'launch', 'lart_bringup', 'dbc_sim.launch.py'], start_new_session=True)
        self.event('status', 'Starting simulation stack (virtual CAN interfaces must already be configured)')

    @staticmethod
    def stop_process(process, sig):
        if process is None or process.poll() is not None:
            return
        os.killpg(process.pid, sig)
        try:
            process.wait(timeout=8)
        except subprocess.TimeoutExpired:
            # Recording may need longer to finalize: leave it alive and report it.
            raise TimeoutError(f'Process {process.pid} is still shutting down')

    def stop_stack(self):
        self.end_test()
        if self.stack is None or self.stack.poll() is not None:
            raise ValueError('This panel does not own a running stack; use Pause for external simulators')
        self.stop_process(self.stack, signal.SIGTERM)
        self.stack = None
        self.discover()
        self.event('status', 'Panel-owned simulation stack stopped')

    def pause(self, name, enabled):
        self.idle_required()
        for target in ([name] if name != 'All' else list(self.profiles)):
            self.set(target, enabled=enabled)

    def apply(self, name, message, signal_name, control, transport):
        self.idle_required()
        profile = self.profiles[name]
        entries = {message: {signal_name: control}}
        parsed, _ = profile['validator'].validate(json.dumps(entries), '{}', 10)
        if transport == 'Direct ROS':
            if control['mode'] == 'auto':
                raise ValueError('Auto applies to CAN simulation; End test releases a direct ROS edit')
            self.begin_direct([(name, message, signal_name, parsed[message][signal_name])])
        else:
            values = self.read(name, ['signal_controls'])
            entries = json.loads(values['signal_controls'])
            entries.setdefault(message, {})[signal_name] = control
            self.set(name, signal_controls=json.dumps(entries))
            self.save_settings(name)

    def timing(self, name, message, default_ms, message_ms):
        self.idle_required()
        if not 1 <= default_ms or not __import__('math').isfinite(default_ms):
            raise ValueError('Default interval must be finite and at least 1 ms')
        intervals = json.loads(self.read(name, ['message_intervals_ms'])['message_intervals_ms'])
        if message_ms is None:
            intervals.pop(message, None)
        else:
            intervals[message] = message_ms
        self.set(name, publish_hz=1000 / default_ms, message_intervals_ms=json.dumps(intervals))
        self.save_settings(name)

    def find_signal(self, message, signal_name, required=True, dbc_stem=None):
        matches = [name for name, p in self.profiles.items() if message in p['validator'].messages
                   and (dbc_stem is None or Path(p['params']['dbc_path']).stem == dbc_stem)
                   and any(s.name == signal_name for s in p['validator'].messages[message].signals)]
        if len(matches) == 1:
            return matches[0]
        if required:
            raise ValueError(f'Need exactly one simulator for {message}/{signal_name}; found {len(matches)}')
        return None

    def fault(self, presets, transport):
        self.idle_required()
        if not presets:
            raise ValueError('Select at least one error preset')
        targets = {}
        for preset in presets:
            if preset not in FAULT_PRESETS:
                raise ValueError(f'Unknown error preset: {preset}')
            for message, sig, value in FAULT_PRESETS[preset][1]:
                name = self.find_signal(message, sig, dbc_stem=FAULT_BUSES[message])
                key = (name, message, sig)
                if key in targets and targets[key]['value'] != value:
                    raise ValueError(f'Conflicting presets for {message}/{sig}; select one condition')
                targets[key] = {'mode': 'fixed', 'value': value}
        prior, updates = {}, {}
        for (name, message, sig), control in targets.items():
            if name not in prior:
                prior[name] = self.read(name, ['enabled', 'signal_controls'])
                updates[name] = json.loads(prior[name]['signal_controls'])
            updates[name].setdefault(message, {})[sig] = control
        for name, controls in updates.items():
            self.profiles[name]['validator'].validate(json.dumps(controls), '{}', 10)
        if transport == 'Direct ROS':
            self.begin_direct([(*key, control) for key, control in targets.items()])
        else:
            self.session = {'kind': 'can', 'restore': prior, 'started': time.monotonic()}
            try:
                for name, controls in updates.items():
                    self.set(name, enabled=True, signal_controls=json.dumps(controls))
            except Exception:
                self.end_test()
                raise
        self.event('status', 'Error test active: ' + ', '.join(presets) + '; Clear errors restores prior settings')

    def mission(self, signal_name, value):
        self.idle_required()
        if value not in range(8):
            raise ValueError('Mission must be 0–7')
        matches = [(n, m.name) for n, p in self.profiles.items() for m in p['db'].messages
                   if any(s.name == signal_name for s in m.signals)]
        if len(matches) != 1:
            raise ValueError(f'Need one simulator containing {signal_name}')
        name, message = matches[0]
        entries = json.loads(self.read(name, ['signal_controls'])['signal_controls'])
        entries.get(message, {}).pop(signal_name, None)
        parameter = 'mission_select_value' if signal_name == 'Mission_select' else 'as_mission_value'
        self.set(name, signal_controls=json.dumps(entries), **{parameter: float(value)})
        self.event('status', f'{signal_name} set to {value}')

    def precharge(self):
        self.idle_required()
        name = self.find_signal('Master_PreCharge_ID_1', 'precharge_state')
        entries = json.loads(self.read(name, ['signal_controls'])['signal_controls'])
        entries.get('Master_PreCharge_ID_1', {}).pop('precharge_state', None)
        self.set(name, signal_controls=json.dumps(entries), precharge_state_value=19.0)
        self.event('status', 'Precharge sequence: 19 → 0–16, every 0.5 seconds')

    def screen(self, index):
        from std_msgs.msg import Int32
        if index not in (0, 1, 2):
            raise ValueError('Screen must be 0–2')
        self.screen_pub.publish(Int32(data=index))
        self.event('status', 'Screen command sent')

    def bag(self):
        if self.recorder and self.recorder.poll() is None:
            self.stop_process(self.recorder, signal.SIGINT)
            self.recorder = None
            self.event('status', f'Recording finalized: {self.record_path}')
            return
        directory = Path(os.environ.get('BAG_DIR', str(Path.home() / 'bags'))).expanduser()
        directory.mkdir(parents=True, exist_ok=True)
        self.record_path = str(directory / time.strftime('datastation_%Y-%m-%dT%H-%M-%S'))
        regex = os.environ.get('BAG_RECORD_REGEX', '/data/.*|/pwt/.*|/can/(?!frames$).*')
        self.recorder = subprocess.Popen(['ros2', 'bag', 'record', '-o', self.record_path, '--regex', regex], start_new_session=True)
        self.event('status', f'Recording: {self.record_path}')

    def begin_direct(self, targets):
        # Capture all messages before pausing any bus; failed setup can be rolled back.
        items = []
        for name, message, sig, control in targets:
            profile = self.profiles[name]
            if message not in profile['topics']:
                raise ValueError(f'No generated ROS message type for {message}; rebuild lart_msgs')
            topic, msg_type = profile['topics'][message]
            if topic not in self.latest:
                raise ValueError(f'Waiting for an initial message on {topic}; resume simulation first')
            if topic not in self.publishers:
                self.publishers[topic] = self.node.create_publisher(msg_type, topic, self.qos)
            signal_def = profile['validator'].messages[message].get_signal_by_name(sig)
            field = ros_name(sig)
            if not hasattr(self.latest[topic], field):
                raise ValueError(f'{topic} has no field {field}')
            items.append((topic, copy.deepcopy(self.latest[topic]), field, signal_def, control))
        self.session = {'kind': 'direct', 'items': items, 'restore': {}, 'started': time.monotonic(), 'next_publish': 0}
        try:
            for name in dict.fromkeys(t[0] for t in targets):
                prior = self.read(name, ['enabled'])['enabled']
                self.session['restore'][name] = {'enabled': prior}
                self.set(name, enabled=False)
        except Exception:
            self.end_test()
            raise
        self.event('status', 'Direct ROS test active; affected simulator buses paused. End test restores them.')

    def scenario(self, scenario_name, custom, step_ms, transport):
        self.idle_required()
        import math
        if not math.isfinite(custom) or not 0 <= custom <= 120 or not math.isfinite(step_ms) or step_ms < 1:
            raise ValueError('Speed must be 0–120 km/h; step interval must be finite and >= 1 ms')
        values = scenario_values(scenario_name, custom)
        pwt = self.find_signal('INV1_ERPM_DUTY_VOLTAGE', 'INV1_Actual_ERPM')
        targets = [(pwt, 'INV1_ERPM_DUTY_VOLTAGE', 'INV1_Actual_ERPM')]
        auto = self.find_signal('DV_dynamics_1', 'Speed_actual', required=False)
        if auto:
            targets.append((auto, 'DV_dynamics_1', 'Speed_actual'))
        if transport == 'Direct ROS':
            self.begin_direct([(n, m, s, {'mode': 'fixed', 'value': 0}) for n, m, s in targets])
        else:
            self.session = {'kind': 'can', 'restore': {}, 'started': time.monotonic()}
            try:
                for name in dict.fromkeys(t[0] for t in targets):
                    self.session['restore'][name] = self.read(name, ['enabled', 'signal_controls'])
                    self.set(name, enabled=True)
            except Exception:
                self.end_test()
                raise
        self.session.update(values=values, index=0, targets=targets, step=step_ms / 1000, next_step=0)
        self.event('status', f'{scenario_name} running via {transport}')

    def tick_session(self, now):
        session = self.session
        if not session:
            return
        if 'values' in session and now >= session['next_step']:
            if session['index'] >= len(session['values']):
                self.end_test()
                return
            speed = session['values'][session['index']]
            session['index'] += 1
            session['next_step'] = now + session['step']
            if session['kind'] == 'can':
                by_node = {}
                for name, message, sig in session['targets']:
                    entries = by_node.setdefault(name, json.loads(self.profiles[name]['params']['signal_controls']))
                    entries.setdefault(message, {})[sig] = {'mode': 'fixed', 'value': speed_to_erpm(speed) if sig == 'INV1_Actual_ERPM' else speed}
                for name, entries in by_node.items():
                    self.set(name, signal_controls=json.dumps(entries))
            else:
                for _, _, field, _, control in session['items']:
                    control['value'] = speed_to_erpm(speed) if field == 'inv1_actual_erpm' else speed
            self.event('status', f'Scenario speed: {speed} km/h ({session["index"]}/{len(session["values"])})')
        if session['kind'] == 'direct' and now >= session['next_publish']:
            outgoing = {}
            for topic, message, field, sig, control in session['items']:
                outgoing.setdefault(topic, copy.deepcopy(message))
                outgoing[topic] = updated_message(outgoing[topic], field, control_value(sig, control, now - session['started']))
            for topic, message in outgoing.items():
                self.publishers[topic].publish(message)
            session['next_publish'] = now + .1

    def end_test(self):
        session = self.session
        if not session:
            return
        # Clear before restoring so failures never leave a publishing test active.
        self.session = None
        errors = []
        for name, values in session['restore'].items():
            try:
                self.set(name, **values)
            except Exception as exc:
                errors.append(f'{name}: {exc}')
        self.event('status', 'Test ended; prior simulator settings restored' if not errors else
                   'Test ended; restore failed: ' + '; '.join(errors))

    def close(self):
        self.closing = True


class Panel:
    def __init__(self, root, backend):
        import tkinter as tk
        from tkinter import ttk
        self.tk, self.ttk = tk, ttk
        self.root, self.backend = root, backend
        self.catalog, self.rows, self.params = {}, {}, {}
        self.bus = tk.StringVar(value='All')
        self.search = tk.StringVar()
        self.transport = tk.StringVar(value='CAN simulation')
        self.mode = tk.StringVar(value='fixed')
        self.status = tk.StringVar(value='Discovering simulators…')
        self.recording = tk.StringVar(value='Not recording')
        self.active = False
        self.edit_widgets = []
        root.title('LART CAN Admin')
        root.geometry('1120x760')
        root.minsize(1120, 620)
        top = ttk.Frame(root, padding=8)
        top.pack(fill='x')
        self.button(top, 'Start / attach', 'start_stack')
        self.button(top, 'Stop owned stack', 'stop_stack')
        self.bus_combo = ttk.Combobox(top, textvariable=self.bus, values=['All'], state='readonly', width=28)
        self.bus_combo.pack(side='left', padx=5)
        self.bus_combo.bind('<<ComboboxSelected>>', lambda _: self.populate())
        self.button(top, 'Pause', 'pause', lambda: (self.bus.get(), False))
        self.button(top, 'Resume', 'pause', lambda: (self.bus.get(), True))
        self.button(top, 'Record / stop bag', 'bag', editable=False)
        ttk.Button(top, text='Help', command=self.help).pack(side='left', padx=3)
        ttk.Label(root, textvariable=self.recording, padding=(8, 0)).pack(anchor='w')
        quick = ttk.LabelFrame(root, text='Dashboard / script controls', padding=8)
        quick.pack(fill='x', padx=8, pady=5)
        for i, label in enumerate(('Driver View', 'Autonomous', 'Driver Gauge')):
            self.button(quick, label, 'screen', lambda i=i: (i,), editable=False)
        for sig in ('Mission_select', 'AS_MISSION'):
            ttk.Label(quick, text=sig).pack(side='left', padx=(8, 2))
            var = tk.StringVar(value='0')
            box = ttk.Combobox(quick, textvariable=var, values=list(range(8)), width=2, state='readonly')
            box.pack(side='left')
            self.button(quick, 'Set', 'mission', lambda sig=sig, var=var: (sig, int(var.get())))
        self.button(quick, 'HV ON sequence', 'precharge')
        scenario = ttk.LabelFrame(root, text='Speed scenarios (km/h)', padding=8)
        scenario.pack(fill='x', padx=8, pady=5)
        self.scenario_name = tk.StringVar(value='Idle')
        ttk.Combobox(scenario, textvariable=self.scenario_name, values=['Idle', 'City', 'Highway', 'Acceleration', 'Deceleration', 'Random', 'Custom'], state='readonly', width=15).pack(side='left')
        self.speed = self.entry(scenario, 'Custom km/h', '30', 5)
        self.step = self.entry(scenario, 'Step ms', '500', 6)
        self.transport_combo = ttk.Combobox(scenario, textvariable=self.transport, values=['CAN simulation', 'Direct ROS'], state='readonly', width=16)
        self.transport_combo.pack(side='left', padx=5)
        self.edit_widgets.append(self.transport_combo)
        self.button(scenario, 'Run', 'scenario', lambda: (self.scenario_name.get(), float(self.speed.get()), float(self.step.get()), self.transport.get()))
        self.button(scenario, 'Cancel / end test', 'end_test', editable=False)
        self.tabs = ttk.Notebook(root)
        self.tabs.pack(fill='both', expand=True, padx=8)
        signals = ttk.Frame(self.tabs)
        errors = ttk.Frame(self.tabs)
        self.tabs.add(signals, text='Signals')
        self.tabs.add(errors, text='Error tests')
        filters = ttk.Frame(signals, padding=8)
        filters.pack(fill='x')
        ttk.Label(filters, text='Search signals').pack(side='left')
        ttk.Entry(filters, textvariable=self.search).pack(side='left', fill='x', expand=True, padx=8)
        self.search.trace_add('write', lambda *_: self.populate())
        listing = ttk.Frame(signals)
        listing.pack(fill='both', expand=True, padx=8)
        columns = ('bus', 'message', 'signal', 'value', 'unit', 'range', 'mode')
        self.tree = ttk.Treeview(listing, columns=columns, show='headings', selectmode='browse')
        for col, width in zip(columns, (160, 200, 240, 90, 50, 120, 65)):
            self.tree.heading(col, text=col.title())
            self.tree.column(col, width=width, minwidth=45)
        scroll = ttk.Scrollbar(listing, orient='vertical', command=self.tree.yview)
        self.tree.configure(yscrollcommand=scroll.set)
        self.tree.pack(side='left', fill='both', expand=True)
        scroll.pack(side='right', fill='y')
        self.tree.bind('<<TreeviewSelect>>', self.selected)
        self.details = tk.StringVar(value='Select a signal to edit. Values use DBC physical units.')
        ttk.Label(signals, textvariable=self.details, wraplength=1080, padding=8).pack(anchor='w')
        editor = ttk.Frame(signals, padding=8)
        editor.pack(fill='x')
        mode_box = ttk.Combobox(editor, textvariable=self.mode, values=['auto', 'fixed', 'sweep', 'random'], state='readonly', width=8)
        mode_box.pack(side='left')
        self.edit_widgets.append(mode_box)
        self.value = self.entry(editor, 'Value', '0', 9)
        self.minimum = self.entry(editor, 'Min', '0', 9)
        self.maximum = self.entry(editor, 'Max', '100', 9)
        self.period = self.entry(editor, 'Cycle seconds', '5', 7)
        self.button(editor, 'Apply signal', 'apply', self.apply_args)
        self.button(editor, 'Reset Auto', 'apply', lambda: (*self.selection(), {'mode': 'auto'}, 'CAN simulation'))
        timing = ttk.Frame(signals, padding=8)
        timing.pack(fill='x')
        self.default_ms = self.entry(timing, 'Bus default ms', '100', 8)
        self.message_ms = self.entry(timing, 'Message ms (blank = default)', '', 8)
        self.button(timing, 'Apply timing', 'timing', self.timing_args)
        self.button(timing, 'Save cfg', 'save_settings')
        self.button(timing, 'Load cfg', 'load_settings')
        ttk.Label(errors, text='Select one or several conditions with Ctrl/Shift-click. Uses the transport selector above. '
                  'Apply holds the faults until Clear errors; conflicting conditions are rejected. '
                  'Tests do not change the saved cfg file.', wraplength=1080, padding=8).pack(anchor='w')
        listing = ttk.Frame(errors)
        listing.pack(fill='both', expand=True, padx=8)
        self.fault_tree = ttk.Treeview(listing, columns=('preset', 'effect'), show='headings', selectmode='extended')
        self.fault_tree.heading('preset', text='Error preset')
        self.fault_tree.heading('effect', text='Test values / expected effect')
        self.fault_tree.column('preset', width=240, minwidth=180)
        self.fault_tree.column('effect', width=820, minwidth=400)
        scroll = ttk.Scrollbar(listing, orient='vertical', command=self.fault_tree.yview)
        self.fault_tree.configure(yscrollcommand=scroll.set)
        self.fault_tree.pack(side='left', fill='both', expand=True)
        scroll.pack(side='right', fill='y')
        for label, (description, _) in FAULT_PRESETS.items():
            self.fault_tree.insert('', 'end', iid=label, values=(label, description))
        self.fault_preview = tk.Text(errors, height=6, wrap='word', state='disabled')
        self.fault_preview.pack(fill='x', padx=8, pady=5)
        self.fault_tree.bind('<<TreeviewSelect>>', self.selected_faults)
        actions = ttk.Frame(errors, padding=8)
        actions.pack(fill='x')
        self.button(actions, 'Apply selected errors', 'fault', self.fault_args)
        self.button(actions, 'Clear errors / restore previous', 'end_test', editable=False)
        self.fault_tree.selection_set('Motor overtemperature')
        ttk.Label(root, textvariable=self.status, padding=8, wraplength=1080).pack(fill='x')
        root.protocol('WM_DELETE_WINDOW', self.close)
        root.after(100, self.poll)

    def fault_args(self):
        selected = list(self.fault_tree.selection())
        if not selected:
            raise ValueError('Select at least one error preset')
        return selected, self.transport.get()

    def selected_faults(self, _=None):
        lines = []
        for label in self.fault_tree.selection():
            description, edits = FAULT_PRESETS[label]
            lines.append(f'{label}: {description}')
            lines.extend(f'  {FAULT_BUSES[message]} / {message} / {sig} = {value:g}'
                         for message, sig, value in edits)
        self.fault_preview.configure(state='normal')
        self.fault_preview.delete('1.0', 'end')
        self.fault_preview.insert('1.0', '\n'.join(lines))
        self.fault_preview.configure(state='disabled')

    def help(self):
        from tkinter.messagebox import showinfo
        showinfo('CAN admin help',
                 'Start / attach connects to the existing DBC simulation stack.\n'
                 'Select a signal to edit its physical value or generated range.\n'
                 'Timing uses milliseconds; blank message timing uses the bus default.\n'
                 'Error tests holds selected fault presets until Clear errors restores prior settings.\n\n'
                 'CAN simulation exercises the full CAN → ROS path.\n'
                 'Direct ROS preserves other message fields and pauses affected buses.\n'
                 'Cancel / end test restores temporary settings.\n\n'
                 'Mission controls use values 0–7. HV ON runs the precharge sequence.\n'
                 'Bag settings use BAG_DIR and BAG_RECORD_REGEX.\n'
                 'Closing stops only processes started by this panel.', parent=self.root)

    def entry(self, parent, label, value, width):
        self.ttk.Label(parent, text=label).pack(side='left', padx=(8, 2))
        var = self.tk.StringVar(value=value)
        widget = self.ttk.Entry(parent, textvariable=var, width=width)
        widget.pack(side='left')
        self.edit_widgets.append(widget)
        return var

    def button(self, parent, label, action, args=lambda: (), editable=True):
        def invoke():
            try:
                self.backend.submit(action, *args())
            except (ValueError, KeyError) as exc:
                self.status.set(f'Error: {exc}')
        widget = self.ttk.Button(parent, text=label, command=invoke)
        widget.pack(side='left', padx=3)
        if editable:
            self.edit_widgets.append(widget)
        return widget

    def selection(self):
        selected = self.tree.selection()
        if not selected:
            raise ValueError('Select a signal first')
        return self.rows[selected[0]]

    def apply_args(self):
        mode = self.mode.get()
        control = {'mode': mode}
        if mode == 'fixed':
            control['value'] = float(self.value.get())
        elif mode in ('sweep', 'random'):
            control.update(min=float(self.minimum.get()), max=float(self.maximum.get()))
            if mode == 'sweep':
                control['period'] = float(self.period.get())
        return (*self.selection(), control, self.transport.get())

    def timing_args(self):
        name, message, _ = self.selection()
        return (name, message, float(self.default_ms.get()),
                float(self.message_ms.get()) if self.message_ms.get().strip() else None)

    def populate(self):
        prior = self.tree.selection()
        self.tree.delete(*self.tree.get_children())
        self.rows.clear()
        query = self.search.get().lower()
        for name, p in self.catalog.items():
            if self.bus.get() not in ('All', name):
                continue
            for message in p['db'].messages:
                for sig in message.signals:
                    if query and query not in f'{name} {message.name} {sig.name}'.lower():
                        continue
                    key = f'{name}/{message.name}/{sig.name}'
                    self.rows[key] = (name, message.name, sig.name)
                    low, high = encodable_range(sig)
                    config = json.loads(self.params.get(name, p['params']).get('signal_controls', '{}'))
                    mode = config.get(message.name, {}).get(sig.name, {}).get('mode', 'auto')
                    self.tree.insert('', 'end', iid=key, values=(name, message.name, sig.name, '—', sig.unit or '', f'{low:g}…{high:g}', mode))
        if prior and prior[0] in self.rows:
            self.tree.selection_set(prior[0])

    def selected(self, _=None):
        if self.active or not self.tree.selection():
            return
        name, message, signal_name = self.selection()
        msg = self.catalog[name]['db'].get_message_by_name(message)
        sig = msg.get_signal_by_name(signal_name)
        params = self.params.get(name, self.catalog[name]['params'])
        control = json.loads(params['signal_controls']).get(message, {}).get(signal_name, {})
        self.mode.set(control.get('mode', 'auto'))
        low, high = encodable_range(sig)
        self.minimum.set(str(control.get('min', low)))
        self.maximum.set(str(control.get('max', high)))
        self.period.set(str(control.get('period', 5)))
        self.value.set(str(control.get('value', low)))
        self.default_ms.set(f'{1000 / params["publish_hz"]:g}')
        self.message_ms.set(str(json.loads(params['message_intervals_ms']).get(message, '')))
        choices = ', '.join(f'{k}: {v}' for k, v in (sig.choices or {}).items())
        if sig.is_multiplexer:
            choices = f'Valid branches: {discrete_values(sig, msg)}'
        branch = f' | branch {sig.multiplexer_signal}={sig.multiplexer_ids}; inactive values transmit when that branch is selected' if sig.multiplexer_signal else ''
        self.details.set(f'{name} | {message}/{signal_name} | {choices or "Continuous/numeric signal"}{branch}')

    def poll(self):
        try:
            while True:
                kind, value = self.backend.events.get_nowait()
                if kind == 'closed':
                    if value:
                        from tkinter.messagebox import showwarning
                        showwarning('Cleanup needs attention', '\n'.join(value), parent=self.root)
                    if getattr(self, 'is_closing', False):
                        self.root.destroy()
                        return
                elif kind == 'status':
                    self.status.set(value)
                elif kind == 'catalog':
                    self.catalog = value
                    self.bus_combo.configure(values=['All', *value])
                    if self.bus.get() not in ['All', *value]:
                        self.bus.set('All')
                    self.populate()
                elif kind == 'params':
                    self.params = value
                    for key, (name, message, sig) in self.rows.items():
                        if name in value:
                            mode = json.loads(value[name]['signal_controls']).get(message, {}).get(sig, {}).get('mode', 'auto')
                            self.tree.set(key, 'mode', mode + (' (paused)' if not value[name]['enabled'] else ''))
                elif kind == 'values':
                    for key, (name, message, sig) in self.rows.items():
                        number = value.get(name, {}).get((message, sig))
                        self.tree.set(key, 'value', f'{number:g}' if number is not None else '— / inactive')
                elif kind == 'activity':
                    active, _, recording, path = value
                    self.active = active
                    for widget in self.edit_widgets:
                        widget.state(['disabled'] if active else ['!disabled'])
                    self.recording.set(f'Recording: {path}' if recording else f'Not recording{": " + path if path else ""}')
        except queue.Empty:
            pass
        self.root.after(100, self.poll)

    def close(self):
        self.is_closing = True
        self.status.set('Restoring test settings and stopping panel-owned processes…')
        self.backend.submit('close')
        if not self.backend.thread.is_alive():
            self.root.destroy()


def main(args=None):
    import tkinter as tk
    root = tk.Tk()
    backend = Backend()
    Panel(root, backend)
    try:
        root.mainloop()
    finally:
        if backend.thread.is_alive():
            backend.submit('close')
            backend.thread.join(timeout=30)


if __name__ == '__main__':
    main()
