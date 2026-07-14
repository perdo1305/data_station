import { Immutable, MessageEvent, PanelExtensionContext, RenderState } from "@foxglove/extension";
import { ReactElement, useEffect, useLayoutEffect, useState } from "react";
import { createRoot } from "react-dom/client";

import { emptyPackState, applyVoltageMessage, applyTemperatureMessage, PackState } from "./packState";
import { classifyVoltage, classifyTemp, severityColor } from "./thresholds";
import { voltageTopics, temperatureTopics } from "./topics";

const VOLTAGE_TOPICS = voltageTopics();
const TEMPERATURE_TOPICS = temperatureTopics();
const VOLTAGE_BY_TOPIC = new Map(VOLTAGE_TOPICS.map((t) => [t.topic, t]));
const TEMPERATURE_BY_TOPIC = new Map(TEMPERATURE_TOPICS.map((t) => [t.topic, t]));

function AmsPackGridPanel({ context }: { context: PanelExtensionContext }): ReactElement {
  const [packState, setPackState] = useState<PackState>(emptyPackState());
  const [renderDone, setRenderDone] = useState<(() => void) | undefined>();

  useLayoutEffect(() => {
    context.onRender = (renderState: Immutable<RenderState>, done) => {
      setRenderDone(() => done);

      const messages = renderState.currentFrame as
        | readonly MessageEvent<Record<string, unknown>>[]
        | undefined;
      if (!messages) {
        return;
      }

      setPackState((prevState) => {
        let nextState = prevState;
        for (const event of messages) {
          const voltageDescriptor = VOLTAGE_BY_TOPIC.get(event.topic);
          if (voltageDescriptor) {
            nextState = applyVoltageMessage(
              nextState,
              voltageDescriptor.module,
              voltageDescriptor.cellOffset,
              event.message,
            );
            continue;
          }
          const tempDescriptor = TEMPERATURE_BY_TOPIC.get(event.topic);
          if (tempDescriptor) {
            nextState = applyTemperatureMessage(
              nextState,
              tempDescriptor.module,
              tempDescriptor.idIndex,
              event.message,
            );
          }
        }
        return nextState;
      });
    };

    context.watch("currentFrame");
    context.subscribe([...VOLTAGE_TOPICS, ...TEMPERATURE_TOPICS].map((t) => ({ topic: t.topic })));
  }, [context]);

  useEffect(() => {
    renderDone?.();
  }, [renderDone]);

  const modules = Object.keys(packState)
    .map(Number)
    .sort((a, b) => a - b);

  return (
    <div style={{ padding: 8, fontFamily: "sans-serif", fontSize: 12, overflow: "auto", height: "100%" }}>
      {modules.map((module) => {
        const moduleState = packState[module];
        if (!moduleState) {
          return undefined;
        }
        const cells = Array.from({ length: 12 }, (_, i) => i + 1);
        const sensors = Array.from({ length: 6 }, (_, i) => i + 1);
        return (
          <div key={module} style={{ display: "flex", alignItems: "center", marginBottom: 4 }}>
            <div style={{ width: 60, fontWeight: "bold" }}>M{module.toString().padStart(2, "0")}</div>
            <div style={{ display: "flex", gap: 2 }}>
              {cells.map((cell) => {
                const volts = moduleState.voltages[cell];
                const severity = classifyVoltage(volts);
                return (
                  <div
                    key={cell}
                    title={volts != undefined ? `Cell ${cell}: ${volts.toFixed(3)} V` : `Cell ${cell}: no data`}
                    style={{
                      width: 40,
                      height: 24,
                      backgroundColor: severityColor(severity),
                      display: "flex",
                      alignItems: "center",
                      justifyContent: "center",
                      color: "#000",
                      borderRadius: 2,
                    }}
                  >
                    {volts != undefined ? volts.toFixed(2) : "--"}
                  </div>
                );
              })}
            </div>
            <div style={{ display: "flex", gap: 2, marginLeft: 12 }}>
              {sensors.map((sensor) => {
                const temp = moduleState.temperatures.sensors[sensor];
                const severity = classifyTemp(temp);
                return (
                  <div
                    key={sensor}
                    title={temp != undefined ? `Sensor ${sensor}: ${temp.toFixed(1)} C` : `Sensor ${sensor}: no data`}
                    style={{
                      width: 36,
                      height: 24,
                      backgroundColor: severityColor(severity),
                      display: "flex",
                      alignItems: "center",
                      justifyContent: "center",
                      color: "#000",
                      borderRadius: 2,
                    }}
                  >
                    {temp != undefined ? temp.toFixed(0) : "--"}
                  </div>
                );
              })}
              <div style={{ marginLeft: 8 }}>
                max {moduleState.temperatures.maximum?.toFixed(1) ?? "--"} / Δ{" "}
                {moduleState.temperatures.delta?.toFixed(2) ?? "--"}
              </div>
            </div>
          </div>
        );
      })}
    </div>
  );
}

export function initAmsPackGrid(context: PanelExtensionContext): () => void {
  const root = createRoot(context.panelElement);
  root.render(<AmsPackGridPanel context={context} />);

  return () => {
    root.unmount();
  };
}
