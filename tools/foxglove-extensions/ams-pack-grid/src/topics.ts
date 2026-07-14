export interface VoltageTopicDescriptor {
  topic: string;
  module: number; // 1-12
  cellOffset: number; // 0, 4, or 8 -- grid cell index = cellOffset + local field number (1-4)
}

export interface TemperatureTopicDescriptor {
  topic: string;
  module: number; // 1-12
  idIndex: 1 | 2;
}

function pad2(n: number): string {
  return n.toString().padStart(2, '0');
}

export function voltageTopics(): VoltageTopicDescriptor[] {
  const topics: VoltageTopicDescriptor[] = [];
  for (let module = 1; module <= 12; module++) {
    for (let idIndex = 1; idIndex <= 3; idIndex++) {
      topics.push({
        topic: `/can/dbc/slave_${pad2(module)}_voltage_id_${idIndex}`,
        module,
        cellOffset: (idIndex - 1) * 4,
      });
    }
  }
  return topics;
}

export function temperatureTopics(): TemperatureTopicDescriptor[] {
  const topics: TemperatureTopicDescriptor[] = [];
  for (let module = 1; module <= 12; module++) {
    for (const idIndex of [1, 2] as const) {
      topics.push({
        topic: `/can/dbc/slave_${pad2(module)}_temperature_id_${idIndex}`,
        module,
        idIndex,
      });
    }
  }
  return topics;
}

export function allTopicNames(): string[] {
  return [...voltageTopics().map((t) => t.topic), ...temperatureTopics().map((t) => t.topic)];
}
