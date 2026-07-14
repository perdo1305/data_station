import { emptyPackState, applyVoltageMessage, applyTemperatureMessage } from './packState';

describe('emptyPackState', () => {
  it('creates all 12 modules with empty voltages/temperatures', () => {
    const state = emptyPackState();
    expect(Object.keys(state)).toHaveLength(12);
    expect(state[1]).toEqual({ voltages: {}, temperatures: { sensors: {} } });
    expect(state[12]).toEqual({ voltages: {}, temperatures: { sensors: {} } });
  });
});

describe('applyVoltageMessage', () => {
  it('maps local field numbers 1-4 to absolute cell numbers via cellOffset', () => {
    let state = emptyPackState();
    state = applyVoltageMessage(state, 1, 0, {
      cell_voltage_1: 3.7,
      cell_voltage_2: 3.71,
      cell_voltage_3: 3.72,
      cell_voltage_4: 3.73,
    });
    expect(state[1].voltages).toEqual({ 1: 3.7, 2: 3.71, 3: 3.72, 4: 3.73 });
  });

  it('offsets cells 5-8 correctly for the second voltage message', () => {
    let state = emptyPackState();
    state = applyVoltageMessage(state, 1, 4, {
      cell_voltage_1: 3.8,
      cell_voltage_2: 3.81,
      cell_voltage_3: 3.82,
      cell_voltage_4: 3.83,
    });
    expect(state[1].voltages).toEqual({ 5: 3.8, 6: 3.81, 7: 3.82, 8: 3.83 });
  });

  it('merges into existing voltages without clobbering other cells', () => {
    let state = emptyPackState();
    state = applyVoltageMessage(state, 1, 0, {
      cell_voltage_1: 3.7,
      cell_voltage_2: 3.71,
      cell_voltage_3: 3.72,
      cell_voltage_4: 3.73,
    });
    state = applyVoltageMessage(state, 1, 4, {
      cell_voltage_1: 3.8,
      cell_voltage_2: 3.81,
      cell_voltage_3: 3.82,
      cell_voltage_4: 3.83,
    });
    expect(Object.keys(state[1].voltages)).toHaveLength(8);
  });

  it('does not affect other modules', () => {
    let state = emptyPackState();
    state = applyVoltageMessage(state, 1, 0, { cell_voltage_1: 3.7 });
    expect(state[2].voltages).toEqual({});
  });
});

describe('applyTemperatureMessage', () => {
  it('extracts sensors 1-4 from idIndex 1', () => {
    let state = emptyPackState();
    state = applyTemperatureMessage(state, 1, 1, {
      temperature_value_1: 25.1,
      temperature_value_2: 25.2,
      temperature_value_3: 25.3,
      temperature_value_4: 25.4,
    });
    expect(state[1].temperatures.sensors).toEqual({ 1: 25.1, 2: 25.2, 3: 25.3, 4: 25.4 });
  });

  it('extracts sensors 5-6 plus maximum/delta from idIndex 2', () => {
    let state = emptyPackState();
    state = applyTemperatureMessage(state, 1, 2, {
      temperature_value_5: 26.1,
      temperature_value_6: 26.2,
      temperature_maximum: 27.0,
      temperature_delta: 1.5,
    });
    expect(state[1].temperatures.sensors).toEqual({ 5: 26.1, 6: 26.2 });
    expect(state[1].temperatures.maximum).toBe(27.0);
    expect(state[1].temperatures.delta).toBe(1.5);
  });

  it('merges idIndex 1 and idIndex 2 data for the same module', () => {
    let state = emptyPackState();
    state = applyTemperatureMessage(state, 1, 1, {
      temperature_value_1: 25.1,
      temperature_value_2: 25.2,
      temperature_value_3: 25.3,
      temperature_value_4: 25.4,
    });
    state = applyTemperatureMessage(state, 1, 2, {
      temperature_value_5: 26.1,
      temperature_value_6: 26.2,
      temperature_maximum: 27.0,
      temperature_delta: 1.5,
    });
    expect(Object.keys(state[1].temperatures.sensors)).toHaveLength(6);
    expect(state[1].temperatures.maximum).toBe(27.0);
  });
});
