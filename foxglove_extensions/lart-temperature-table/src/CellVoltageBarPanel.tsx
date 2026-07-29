import { PanelExtensionContext, SettingsTreeAction, SettingsTree } from "@foxglove/extension";
import { ReactElement, useCallback, useEffect, useLayoutEffect, useMemo, useState } from "react";
import { createRoot } from "react-dom/client";

import { Bar, BarChart } from "./BarChart";

type Config = {
  topicPrefix: string;
  slaveCount: number;
  yMin: number;
  yMax: number;
  undervoltage: number;
  overvoltage: number;
};

const DEFAULT_CONFIG: Config = {
  topicPrefix: "/can/dbc/slave_",
  slaveCount: 12,
  yMin: 3,
  yMax: 4.3,
  undervoltage: 3,
  overvoltage: 4.2,
};

type VoltId1 = { cell_voltage_1: number; cell_voltage_2: number; cell_voltage_3: number; cell_voltage_4: number };
type VoltId2 = { cell_voltage_5: number; cell_voltage_6: number; cell_voltage_7: number; cell_voltage_8: number };
type VoltId3 = {
  cell_voltage_9: number;
  cell_voltage_10: number;
  cell_voltage_11: number;
  cell_voltage_12: number;
};

function pad2(n: number): string {
  return n < 10 ? `0${n}` : `${n}`;
}

function topicsForSlave(config: Config, n: number): { id1: string; id2: string; id3: string } {
  const p = `${config.topicPrefix}${pad2(n)}`;
  return { id1: `${p}_voltage_id_1`, id2: `${p}_voltage_id_2`, id3: `${p}_voltage_id_3` };
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
          yMin: { label: "Y min (V)", input: "number", value: config.yMin },
          yMax: { label: "Y max (V)", input: "number", value: config.yMax },
          undervoltage: { label: "Undervoltage (V)", input: "number", value: config.undervoltage },
          overvoltage: { label: "Overvoltage (V)", input: "number", value: config.overvoltage },
        },
      },
    },
  };
}

function CellVoltageBarPanel({ context }: { context: PanelExtensionContext }): ReactElement {
  const [config, setConfig] = useState<Config>({
    ...DEFAULT_CONFIG,
    ...(context.initialState as Partial<Config> | undefined),
  });
  const [readings, setReadings] = useState<Record<string, unknown>>({});
  const [renderDone, setRenderDone] = useState<(() => void) | undefined>();

  const slaveTopics = useMemo(
    () => Array.from({ length: config.slaveCount }, (_, i) => topicsForSlave(config, i + 1)),
    [config],
  );

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
    const subs = slaveTopics.flatMap((t) => [{ topic: t.id1 }, { topic: t.id2 }, { topic: t.id3 }]);
    context.subscribe(subs);
  }, [context, slaveTopics]);

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

  const bars: Bar[] = [];
  slaveTopics.forEach((t, idx) => {
    const n = idx + 1;
    const id1 = readings[t.id1] as VoltId1 | undefined;
    const id2 = readings[t.id2] as VoltId2 | undefined;
    const id3 = readings[t.id3] as VoltId3 | undefined;
    const cells = [
      id1?.cell_voltage_1,
      id1?.cell_voltage_2,
      id1?.cell_voltage_3,
      id1?.cell_voltage_4,
      id2?.cell_voltage_5,
      id2?.cell_voltage_6,
      id2?.cell_voltage_7,
      id2?.cell_voltage_8,
      id3?.cell_voltage_9,
      id3?.cell_voltage_10,
      id3?.cell_voltage_11,
      id3?.cell_voltage_12,
    ];
    cells.forEach((v, cellIdx) => {
      bars.push({ label: `Slave ${pad2(n)} cell ${cellIdx + 1}`, value: v, groupBoundary: cellIdx === 0 });
    });
  });

  const colorFor = (v: number): string => {
    if (v < config.undervoltage || v > config.overvoltage) {
      return "#ff4d4d";
    }
    return "#3cb45a";
  };

  const receivedCells = bars.filter((b) => b.value != undefined).length;

  return (
    <div style={{ padding: "0.75rem", height: "100%", boxSizing: "border-box", display: "flex", flexDirection: "column", color: "#fff" }}>
      <div style={{ marginBottom: 6, opacity: 0.7, fontSize: 12, fontFamily: "monospace" }}>
        {receivedCells}/{bars.length} cells populated &middot; range {config.yMin}&ndash;{config.yMax}V &middot; UV{" "}
        {config.undervoltage}V / OV {config.overvoltage}V
      </div>
      <div style={{ flex: 1 }}>
        <BarChart bars={bars} yMin={config.yMin} yMax={config.yMax} colorFor={colorFor} unit="V" />
      </div>
    </div>
  );
}

export function initCellVoltageBarPanel(context: PanelExtensionContext): () => void {
  const root = createRoot(context.panelElement);
  root.render(<CellVoltageBarPanel context={context} />);
  return () => {
    root.unmount();
  };
}
