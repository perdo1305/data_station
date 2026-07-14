import { voltageTopics, temperatureTopics, allTopicNames } from './topics';

describe('voltageTopics', () => {
  it('produces 36 descriptors (12 modules x 3 messages)', () => {
    expect(voltageTopics()).toHaveLength(36);
  });
  it('zero-pads module numbers and sets correct cellOffset', () => {
    const topics = voltageTopics();
    expect(topics[0]).toEqual({
      topic: '/can/dbc/slave_01_voltage_id_1',
      module: 1,
      cellOffset: 0,
    });
    expect(topics[1]).toEqual({
      topic: '/can/dbc/slave_01_voltage_id_2',
      module: 1,
      cellOffset: 4,
    });
    expect(topics[2]).toEqual({
      topic: '/can/dbc/slave_01_voltage_id_3',
      module: 1,
      cellOffset: 8,
    });
  });
  it('reaches module 12', () => {
    const topics = voltageTopics();
    expect(topics[topics.length - 1]).toEqual({
      topic: '/can/dbc/slave_12_voltage_id_3',
      module: 12,
      cellOffset: 8,
    });
  });
});

describe('temperatureTopics', () => {
  it('produces 24 descriptors (12 modules x 2 messages)', () => {
    expect(temperatureTopics()).toHaveLength(24);
  });
  it('zero-pads module numbers and sets correct idIndex', () => {
    const topics = temperatureTopics();
    expect(topics[0]).toEqual({
      topic: '/can/dbc/slave_01_temperature_id_1',
      module: 1,
      idIndex: 1,
    });
    expect(topics[1]).toEqual({
      topic: '/can/dbc/slave_01_temperature_id_2',
      module: 1,
      idIndex: 2,
    });
  });
});

describe('allTopicNames', () => {
  it('combines voltage and temperature topics with no duplicates', () => {
    const names = allTopicNames();
    expect(names).toHaveLength(60);
    expect(new Set(names).size).toBe(60);
  });
});
