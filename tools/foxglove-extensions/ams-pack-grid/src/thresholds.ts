export type Severity = 'ok' | 'warning' | 'critical' | 'nodata';

export const VOLTAGE_OK_MIN = 3.4;
export const VOLTAGE_OK_MAX = 4.15;
export const VOLTAGE_WARN_MIN = 3.2;
export const VOLTAGE_WARN_MAX = 4.2;

export function classifyVoltage(volts: number | undefined): Severity {
  if (volts == undefined) {return 'nodata';}
  if (volts >= VOLTAGE_OK_MIN && volts <= VOLTAGE_OK_MAX) {return 'ok';}
  if (volts >= VOLTAGE_WARN_MIN && volts <= VOLTAGE_WARN_MAX) {return 'warning';}
  return 'critical';
}

export const TEMP_OK_MAX = 45;
export const TEMP_WARN_MAX = 60;

export function classifyTemp(celsius: number | undefined): Severity {
  if (celsius == undefined) {return 'nodata';}
  if (celsius < TEMP_OK_MAX) {return 'ok';}
  if (celsius <= TEMP_WARN_MAX) {return 'warning';}
  return 'critical';
}

export function severityColor(severity: Severity): string {
  switch (severity) {
    case 'ok':
      return '#2ecc71';
    case 'warning':
      return '#f1c40f';
    case 'critical':
      return '#e74c3c';
    case 'nodata':
      return '#7f8c8d';
  }
}
