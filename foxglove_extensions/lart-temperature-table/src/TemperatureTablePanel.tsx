import { PanelExtensionContext, SettingsTreeAction, SettingsTree } from "@foxglove/extension";
import { ReactElement, useCallback, useEffect, useLayoutEffect, useMemo, useState } from "react";
import { createRoot } from "react-dom/client";

type Config = {
  topicPrefix: string;
  slaveCount: number;
  minTemp: number;
  lowWarnTemp: number;
  highWarnTemp: number;
  maxTemp: number;
};

const DEFAULT_CONFIG: Config = {
  topicPrefix: "/can/dbc/slave_",
  slaveCount: 12,
  minTemp: 10,
  lowWarnTemp: 20,
  highWarnTemp: 50,
  maxTemp: 60,
};

// Slave_01_temperature_id_1 -> temperature_value_1..4
// Slave_01_temperature_id_2 -> temperature_delta, temperature_maximum, temperature_value_5..6
type Id1Msg = {
  temperature_value_1: number;
  temperature_value_2: number;
  temperature_value_3: number;
  temperature_value_4: number;
};
type Id2Msg = {
  temperature_delta: number;
  temperature_maximum: number;
  temperature_value_5: number;
  temperature_value_6: number;
};

function pad2(n: number): string {
  return n < 10 ? `0${n}` : `${n}`;
}

function topicsForConfig(config: Config): { id1: string; id2: string }[] {
  const out: { id1: string; id2: string }[] = [];
  for (let n = 1; n <= config.slaveCount; n++) {
    out.push({
      id1: `${config.topicPrefix}${pad2(n)}_temperature_id_1`,
      id2: `${config.topicPrefix}${pad2(n)}_temperature_id_2`,
    });
  }
  return out;
}

function cellColor(value: number | undefined, config: Config): string {
  if (value == undefined) {
    return "transparent";
  }
  if (value < config.lowWarnTemp) {
    return "#4682ff";
  }
  if (value > config.highWarnTemp) {
    return "#ff7846";
  }
  return "#3cb45a";
}

function buildSettingsTree(config: Config, actionHandler: (action: SettingsTreeAction) => void): SettingsTree {
  return {
    actionHandler,
    nodes: {
      general: {
        label: "Settings",
        fields: {
          topicPrefix: { label: "Topic prefix", input: "string", value: config.topicPrefix },
          slaveCount: {
            label: "Slave count",
            input: "number",
            value: config.slaveCount,
            min: 1,
            max: 32,
            step: 1,
            precision: 0,
          },
          minTemp: { label: "Min temp (ºC)", input: "number", value: config.minTemp },
          lowWarnTemp: { label: "Low warn (ºC)", input: "number", value: config.lowWarnTemp },
          highWarnTemp: { label: "High warn (ºC)", input: "number", value: config.highWarnTemp },
          maxTemp: { label: "Max temp (ºC)", input: "number", value: config.maxTemp },
        },
      },
    },
  };
}

function TemperatureTablePanel({ context }: { context: PanelExtensionContext }): ReactElement {
  const [config, setConfig] = useState<Config>({
    ...DEFAULT_CONFIG,
    ...(context.initialState as Partial<Config> | undefined),
  });
  const [readings, setReadings] = useState<Record<string, unknown>>({});
  const [renderDone, setRenderDone] = useState<(() => void) | undefined>();

  const topics = useMemo(() => topicsForConfig(config), [config]);

  useLayoutEffect(() => {
    context.onRender = (renderState, done) => {
      setRenderDone(() => done);
      const frame = renderState.currentFrame;
      if (frame && frame.length > 0) {
        setReadings((prev) => {
          const next = { ...prev };
          for (const ev of frame) {
            next[ev.topic] = ev.message;
          }
          return next;
        });
      }
    };
    context.watch("currentFrame");
    context.watch("topics");
  }, [context]);

  useEffect(() => {
    const subs = topics.flatMap((t) => [{ topic: t.id1 }, { topic: t.id2 }]);
    context.subscribe(subs);
  }, [context, topics]);

  useEffect(() => {
    renderDone?.();
  }, [renderDone]);

  const updatePanelSettings = useCallback(
    (action: SettingsTreeAction) => {
      if (action.action !== "update") {
        return;
      }
      const { path, value } = action.payload;
      setConfig((prev) => {
        const next = { ...prev, [path[path.length - 1]!]: value };
        context.saveState(next);
        return next;
      });
    },
    [context],
  );

  useEffect(() => {
    context.updatePanelSettingsEditor(buildSettingsTree(config, updatePanelSettings));
  }, [context, config, updatePanelSettings]);

  const rows = topics.map((t, idx) => {
    const n = idx + 1;
    const id1 = readings[t.id1] as Id1Msg | undefined;
    const id2 = readings[t.id2] as Id2Msg | undefined;
    return {
      slave: n,
      t1: id1?.temperature_value_1,
      t2: id1?.temperature_value_2,
      t3: id1?.temperature_value_3,
      t4: id1?.temperature_value_4,
      t5: id2?.temperature_value_5,
      t6: id2?.temperature_value_6,
      max: id2?.temperature_maximum,
      delta: id2?.temperature_delta,
    };
  });

  const receivedCount = topics.filter((t) => readings[t.id1] != undefined || readings[t.id2] != undefined).length;

  const th: React.CSSProperties = {
    textAlign: "left",
    padding: "4px 10px",
    borderBottom: "1px solid #3a3a3a",
    position: "sticky",
    top: 0,
    background: "#1a1a1a",
  };
  const td: React.CSSProperties = { padding: "3px 10px", fontVariantNumeric: "tabular-nums" };

  return (
    <div style={{ padding: "0.75rem", fontFamily: "monospace", color: "#fff", overflow: "auto", height: "100%" }}>
      <div style={{ marginBottom: 6, opacity: 0.7, fontSize: 12 }}>
        {receivedCount}/{config.slaveCount} slaves reporting &middot; scale {config.minTemp}
        &ndash;{config.maxTemp}&deg;C &middot; warn below {config.lowWarnTemp}&deg;C / above{" "}
        {config.highWarnTemp}&deg;C
      </div>
      <table style={{ borderCollapse: "collapse", width: "100%" }}>
        <thead>
          <tr>
            <th style={th}>Slave</th>
            {["T1", "T2", "T3", "T4", "T5", "T6"].map((h) => (
              <th key={h} style={th}>
                {h}
              </th>
            ))}
            <th style={th}>Max</th>
            <th style={th}>Delta</th>
          </tr>
        </thead>
        <tbody>
          {rows.map((r) => (
            <tr key={r.slave}>
              <td style={{ ...td, fontWeight: "bold" }}>Slave {pad2(r.slave)}</td>
              {[r.t1, r.t2, r.t3, r.t4, r.t5, r.t6].map((v, i) => (
                <td key={i} style={{ ...td, background: cellColor(v, config) }}>
                  {v != undefined ? v.toFixed(1) : "—"}
                </td>
              ))}
              <td style={{ ...td, background: cellColor(r.max, config) }}>
                {r.max != undefined ? r.max.toFixed(1) : "—"}
              </td>
              <td style={td}>{r.delta != undefined ? r.delta.toFixed(2) : "—"}</td>
            </tr>
          ))}
        </tbody>
      </table>
    </div>
  );
}

export function initTemperatureTablePanel(context: PanelExtensionContext): () => void {
  const root = createRoot(context.panelElement);
  root.render(<TemperatureTablePanel context={context} />);
  return () => {
    root.unmount();
  };
}
