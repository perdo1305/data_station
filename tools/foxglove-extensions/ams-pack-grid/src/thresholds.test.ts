import { classifyVoltage, classifyTemp, severityColor } from './thresholds';

describe('classifyVoltage', () => {
  it('returns nodata for undefined', () => {
    expect(classifyVoltage(undefined)).toBe('nodata');
  });
  it('returns ok in the healthy band', () => {
    expect(classifyVoltage(3.4)).toBe('ok');
    expect(classifyVoltage(3.7)).toBe('ok');
    expect(classifyVoltage(4.15)).toBe('ok');
  });
  it('returns warning just outside the healthy band', () => {
    expect(classifyVoltage(3.2)).toBe('warning');
    expect(classifyVoltage(3.39)).toBe('warning');
    expect(classifyVoltage(4.16)).toBe('warning');
    expect(classifyVoltage(4.2)).toBe('warning');
  });
  it('returns critical outside the warning band', () => {
    expect(classifyVoltage(3.1)).toBe('critical');
    expect(classifyVoltage(4.21)).toBe('critical');
    expect(classifyVoltage(0)).toBe('critical');
  });
});

describe('classifyTemp', () => {
  it('returns nodata for undefined', () => {
    expect(classifyTemp(undefined)).toBe('nodata');
  });
  it('returns ok below 45', () => {
    expect(classifyTemp(20)).toBe('ok');
    expect(classifyTemp(44.9)).toBe('ok');
  });
  it('returns warning between 45 and 60 inclusive', () => {
    expect(classifyTemp(45)).toBe('warning');
    expect(classifyTemp(60)).toBe('warning');
  });
  it('returns critical above 60', () => {
    expect(classifyTemp(60.1)).toBe('critical');
    expect(classifyTemp(90)).toBe('critical');
  });
});

describe('severityColor', () => {
  it('maps every severity to a distinct color', () => {
    const colors = new Set([
      severityColor('ok'),
      severityColor('warning'),
      severityColor('critical'),
      severityColor('nodata'),
    ]);
    expect(colors.size).toBe(4);
  });
});
