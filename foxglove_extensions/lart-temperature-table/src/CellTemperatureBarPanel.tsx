import { PanelExtensionContext, SettingsTreeAction, SettingsTree } from "@foxglove/extension";
import { ReactElement, useCallback, useEffect, useLayoutEffect, useMemo, useState } from "react";
import { createRoot } from "react-dom/client";

import { Bar, BarChart } from "./BarChart";

type Config = {
  topicPrefix: string;
  slaveCount: number;
  yMin: number;
  yMax: number;
  overtemperature: number;
};

const DEFAULT_CONFIG: Config = {
  topicPrefix: "/can/dbc/slave_",
  slaveCount: 12,
  yMin: 5,
  yMax: 70,
  overtemperature: 60,
};

type TempId1 = {
  temperature_value_1: number;
  temperature_value_2: number;
  temperature_value_3: number;
  temperature_value_4: number;
};
type TempId2 = { temperature_value_5: number; temperature_value_6: number };

function pad2(n: number): string {
  return n < 10 ? `0${n}` : `${n}`;
}

function topicsForSlave(config: Config, n: number): { id1: string; id2: string } {
  const p = `${config.topicPrefix}${pad2(n)}`;
  return { id1: `${p}_temperature_id_1`, id2: `${p}_temperature_id_2` };
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
          yMin: { label: "Y min (ºC)", input: "number", value: config.yMin },
          yMax: { label: "Y max (ºC)", input: "number", value: config.yMax },
          overtemperature: { label: "Overtemperature (ºC)", input: "number", value: config.overtemperature },
        },
      },
    },
  };
}

function CellTemperatureBarPanel({ context }: { context: PanelExtensionContext }): ReactElement {
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
    const subs = slaveTopics.flatMap((t) => [{ topic: t.id1 }, { topic: t.id2 }]);
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
    const id1 = readings[t.id1] as TempId1 | undefined;
    const id2 = readings[t.id2] as TempId2 | undefined;
    const temps = [
      id1?.temperature_value_1,
      id1?.temperature_value_2,
      id1?.temperature_value_3,
      id1?.temperature_value_4,
      id2?.temperature_value_5,
      id2?.temperature_value_6,
    ];
    temps.forEach((v, tIdx) => {
      bars.push({ label: `Slave ${pad2(n)} T${tIdx + 1}`, value: v, groupBoundary: tIdx === 0 });
    });
  });

  const colorFor = (v: number): string => {
    if (v > config.overtemperature) {
      return "#ff4d4d";
    }
    if (v < config.yMin + (config.yMax - config.yMin) * 0.15) {
      return "#4682ff";
    }
    return "#3cb45a";
  };

  const receivedCells = bars.filter((b) => b.value != undefined).length;

  return (
    <div style={{ padding: "0.75rem", height: "100%", boxSizing: "border-box", display: "flex", flexDirection: "column", color: "#fff" }}>
      <div style={{ marginBottom: 6, opacity: 0.7, fontSize: 12, fontFamily: "monospace" }}>
        {receivedCells}/{bars.length} thermistors populated &middot; range {config.yMin}&ndash;{config.yMax}&deg;C
        &middot; overtemp {config.overtemperature}&deg;C
      </div>
      <div style={{ flex: 1 }}>
        <BarChart bars={bars} yMin={config.yMin} yMax={config.yMax} colorFor={colorFor} unit="ºC" />
      </div>
    </div>
  );
}

export function initCellTemperatureBarPanel(context: PanelExtensionContext): () => void {
  const root = createRoot(context.panelElement);
  root.render(<CellTemperatureBarPanel context={context} />);
  return () => {
    root.unmount();
  };
}
