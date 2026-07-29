import { ExtensionContext } from "@foxglove/extension";

import { initCellTemperatureBarPanel } from "./CellTemperatureBarPanel";
import { initCellVoltageBarPanel } from "./CellVoltageBarPanel";
import { initTemperatureTablePanel } from "./TemperatureTablePanel";
import { initValueTilePanel } from "./ValueTilePanel";

export function activate(extensionContext: ExtensionContext): void {
  extensionContext.registerPanel({
    name: "AMS Temperature Table (LART)",
    initPanel: initTemperatureTablePanel,
  });
  extensionContext.registerPanel({
    name: "AMS Cell Voltage Bar (LART)",
    initPanel: initCellVoltageBarPanel,
  });
  extensionContext.registerPanel({
    name: "AMS Cell Temperature Bar (LART)",
    initPanel: initCellTemperatureBarPanel,
  });
  extensionContext.registerPanel({
    name: "AMS Value Tile (LART)",
    initPanel: initValueTilePanel,
  });
}
