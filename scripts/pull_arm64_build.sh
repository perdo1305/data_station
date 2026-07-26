#!/usr/bin/env bash
set -euo pipefail

REPO="perdo1305/data_station"
TAG="latest-arm64"
ASSET="lart-dashboard-arm64.tar.gz"
WS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

TMP="$(mktemp -d)"
trap 'rm -rf "$TMP"' EXIT

echo "Downloading ${ASSET} from ${REPO}@${TAG}..."
curl -fsSL "https://github.com/${REPO}/releases/download/${TAG}/${ASSET}" -o "$TMP/${ASSET}"

echo "Removing old build output..."
rm -rf "$WS_DIR/install" "$WS_DIR/LART_Car_Dashboard_v1/build/ui-build"

echo "Extracting into ${WS_DIR}..."
tar xzf "$TMP/${ASSET}" -C "$WS_DIR"

echo "Done. Run: source $WS_DIR/install/setup.bash"
