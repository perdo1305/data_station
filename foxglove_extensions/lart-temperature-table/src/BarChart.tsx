import { ReactElement } from "react";

export type Bar = {
  label: string;
  value: number | undefined;
  groupBoundary?: boolean; // true = start a new visual group (extra gap + tick)
};

export function BarChart({
  bars,
  yMin,
  yMax,
  colorFor,
  unit,
}: {
  bars: Bar[];
  yMin: number;
  yMax: number;
  colorFor: (value: number) => string;
  unit: string;
}): ReactElement {
  const rawRange = yMax - yMin;
  const range = rawRange === 0 ? 1 : rawRange;
  return (
    <div style={{ display: "flex", alignItems: "flex-end", height: "100%", gap: 1, overflowX: "auto" }}>
      {bars.map((b, i) => {
        const has = b.value != undefined;
        const isGroupStart = b.groupBoundary === true && i > 0;
        const clamped = has ? Math.max(yMin, Math.min(yMax, b.value!)) : yMin;
        const heightPct = ((clamped - yMin) / range) * 100;
        return (
          <div
            key={i}
            title={`${b.label}: ${has ? b.value!.toFixed(2) : "no data"} ${unit}`}
            style={{
              flex: "1 0 4px",
              minWidth: 4,
              maxWidth: 14,
              height: "100%",
              display: "flex",
              alignItems: "flex-end",
              marginLeft: isGroupStart ? 4 : 0,
              borderLeft: isGroupStart ? "1px solid #444" : undefined,
              paddingLeft: isGroupStart ? 3 : 0,
            }}
          >
            <div
              style={{
                width: "100%",
                height: `${has ? Math.max(1, heightPct) : 1}%`,
                background: has ? colorFor(b.value!) : "#3a3a3a",
                borderRadius: "1px 1px 0 0",
              }}
            />
          </div>
        );
      })}
    </div>
  );
}
