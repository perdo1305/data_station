#!/usr/bin/env bash
set -euo pipefail

REPO="perdo1305/data_station"
TAG="latest-arm64"
ASSET="lart-dashboard-arm64.tar.gz"
WS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
UI_BUILD="LART_Car_Dashboard_v1/build/ui-build"

# Stage inside the workspace so the final mv stays on one filesystem.
TMP="$(mktemp -d "$WS_DIR/.arm64-pull.XXXXXX")"
trap 'rm -rf "$TMP"' EXIT

echo "Downloading ${ASSET} from ${REPO}@${TAG}..."
curl -fsSL "https://github.com/${REPO}/releases/download/${TAG}/${ASSET}" -o "$TMP/${ASSET}"

echo "Extracting into staging..."
mkdir "$TMP/stage"
tar xzf "$TMP/${ASSET}" -C "$TMP/stage"

if ! bash "$WS_DIR/scripts/check_arm64_bundle.sh" "$TMP/stage"; then
    echo "Downloaded build is incomplete; keeping the current build." >&2
    exit 1
fi

echo "Replacing old build output..."
rm -rf "$WS_DIR/install" "$WS_DIR/$UI_BUILD"
mkdir -p "$(dirname "$WS_DIR/$UI_BUILD")"
mv "$TMP/stage/install" "$WS_DIR/install"
mv "$TMP/stage/$UI_BUILD" "$WS_DIR/$UI_BUILD"

echo "Done. Run: source $WS_DIR/install/setup.bash"
