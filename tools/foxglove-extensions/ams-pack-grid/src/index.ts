import { ExtensionContext } from "@foxglove/extension";

import { initAmsPackGrid } from "./AmsPackGrid";

export function activate(extensionContext: ExtensionContext): void {
  extensionContext.registerPanel({ name: "AMS Pack Grid", initPanel: initAmsPackGrid });
}
