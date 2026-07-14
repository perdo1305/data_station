export interface ModuleTemperatures {
  sensors: Record<number, number>; // sensor number 1-6 -> celsius
  maximum?: number;
  delta?: number;
}

export interface ModuleState {
  voltages: Record<number, number>; // cell number 1-12 -> volts
  temperatures: ModuleTemperatures;
}

export type PackState = Record<number, ModuleState>;

export function emptyPackState(): PackState {
  const state: PackState = {};
  for (let module = 1; module <= 12; module++) {
    state[module] = { voltages: {}, temperatures: { sensors: {} } };
  }
  return state;
}

function isNumber(value: unknown): value is number {
  return typeof value === 'number';
}

export function applyVoltageMessage(
  state: PackState,
  module: number,
  cellOffset: number,
  msg: Record<string, unknown>,
): PackState {
  const newVoltages: Record<number, number> = { ...state[module].voltages };
  for (let local = 1; local <= 4; local++) {
    const value = msg[`cell_voltage_${local}`];
    if (isNumber(value)) {
      newVoltages[cellOffset + local] = value;
    }
  }
  return {
    ...state,
    [module]: {
      ...state[module],
      voltages: newVoltages,
    },
  };
}

export function applyTemperatureMessage(
  state: PackState,
  module: number,
  idIndex: 1 | 2,
  msg: Record<string, unknown>,
): PackState {
  const existing = state[module].temperatures;
  const newSensors: Record<number, number> = { ...existing.sensors };
  let maximum = existing.maximum;
  let delta = existing.delta;

  if (idIndex === 1) {
    for (let local = 1; local <= 4; local++) {
      const value = msg[`temperature_value_${local}`];
      if (isNumber(value)) {
        newSensors[local] = value;
      }
    }
  } else {
    const v5 = msg['temperature_value_5'];
    const v6 = msg['temperature_value_6'];
    if (isNumber(v5)) newSensors[5] = v5;
    if (isNumber(v6)) newSensors[6] = v6;
    const max = msg['temperature_maximum'];
    const del = msg['temperature_delta'];
    if (isNumber(max)) maximum = max;
    if (isNumber(del)) delta = del;
  }

  return {
    ...state,
    [module]: {
      ...state[module],
      temperatures: { sensors: newSensors, maximum, delta },
    },
  };
}
