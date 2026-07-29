import { PanelExtensionContext, SettingsTreeAction, SettingsTree } from "@foxglove/extension";
import { ReactElement, useCallback, useEffect, useLayoutEffect, useState } from "react";
import { createRoot } from "react-dom/client";

type Config = {
  topic: string;
  field: string;
  label: string;
  unit: string;
  precision: number;
  lowCrit: number;
  lowWarn: number;
  highWarn: number;
  highCrit: number;
};

// +/-999999 stand in for "no bound" — plain JSON has no Infinity literal, and
// a saved layout containing one is rejected by Foxglove on load.
const NO_BOUND = 999999;
const DEFAULT_CONFIG: Config = {
  topic: "",
  field: "",
  label: "Value",
  unit: "",
  precision: 2,
  lowCrit: -NO_BOUND,
  lowWarn: -NO_BOUND,
  highWarn: NO_BOUND,
  highCrit: NO_BOUND,
};

function colorFor(v: number, c: Config): string {
  if (v <= c.lowCrit || v >= c.highCrit) {
    return "#c0392b"; // red — out of safe range
  }
  if (v <= c.lowWarn || v >= c.highWarn) {
    return "#d98a1f"; // orange — approaching limit
  }
  return "#1f7a3d"; // green — normal
}

function buildSettingsTree(config: Config, actionHandler: (action: SettingsTreeAction) => void): SettingsTree {
  return {
    actionHandler,
    nodes: {
      general: {
        label: "Settings",
        fields: {
          label: { label: "Label", input: "string", value: config.label },
          topic: { label: "Topic", input: "string", value: config.topic },
          field: { label: "Field", input: "string", value: config.field },
          unit: { label: "Unit", input: "string", value: config.unit },
          precision: { label: "Precision", input: "number", value: config.precision, min: 0, max: 6, step: 1, precision: 0 },
        },
      },
      thresholds: {
        label: "Warning thresholds",
        fields: {
          lowCrit: { label: "Low critical (red)", input: "number", value: config.lowCrit },
          lowWarn: { label: "Low warn (orange)", input: "number", value: config.lowWarn },
          highWarn: { label: "High warn (orange)", input: "number", value: config.highWarn },
          highCrit: { label: "High critical (red)", input: "number", value: config.highCrit },
        },
      },
    },
  };
}

function ValueTilePanel({ context }: { context: PanelExtensionContext }): ReactElement {
  const [config, setConfig] = useState<Config>({
    ...DEFAULT_CONFIG,
    ...(context.initialState as Partial<Config> | undefined),
  });
  const [value, setValue] = useState<number | undefined>(undefined);
  const [renderDone, setRenderDone] = useState<(() => void) | undefined>();

  useLayoutEffect(() => {
    context.onRender = (renderState, done) => {
      setRenderDone(() => done);
      const frame = renderState.currentFrame;
      if (frame && frame.length > 0) {
        const last = frame[frame.length - 1];
        if (last) {
          const msg = last.message as Record<string, unknown>;
          const v = msg[config.field];
          if (typeof v === "number") {
            setValue(v);
          }
        }
      }
    };
    context.watch("currentFrame");
    context.watch("topics");
  }, [context, config.field]);

  useEffect(() => {
    if (config.topic) {
      context.subscribe([{ topic: config.topic }]);
    }
  }, [context, config.topic]);

  useEffect(() => {
    renderDone?.();
  }, [renderDone]);

  const updatePanelSettings = useCallback(
    (action: SettingsTreeAction) => {
      if (action.action !== "update") {
        return;
      }
      const { path, value: newValue } = action.payload;
      setConfig((prev) => {
        const next = { ...prev, [path[path.length - 1]!]: newValue };
        context.saveState(next);
        return next;
      });
    },
    [context],
  );

  useEffect(() => {
    context.updatePanelSettingsEditor(buildSettingsTree(config, updatePanelSettings));
  }, [context, config, updatePanelSettings]);

  const background = value != undefined ? colorFor(value, config) : "#2a2a2a";

  return (
    <div
      style={{
        height: "100%",
        width: "100%",
        boxSizing: "border-box",
        display: "flex",
        alignItems: "center",
        justifyContent: "center",
        background,
        color: "#fff",
        fontFamily: "monospace",
        fontWeight: "bold",
        fontSize: 28,
        transition: "background 0.15s ease",
      }}
    >
      {value != undefined ? `${value.toFixed(config.precision)}${config.unit}` : "—"}
    </div>
  );
}

export function initValueTilePanel(context: PanelExtensionContext): () => void {
  const root = createRoot(context.panelElement);
  root.render(<ValueTilePanel context={context} />);
  return () => {
    root.unmount();
  };
}
