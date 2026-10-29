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

# Put $TMP/old/$1 back at $2, replacing the partial new tree there; no-op if it was never moved aside.
restore() {
    [ ! -e "$TMP/old/$1" ] || { rm -rf "$2" && mv "$TMP/old/$1" "$2"; }
}

echo "Replacing old build output..."
mkdir -p "$TMP/old" "$(dirname "$WS_DIR/$UI_BUILD")"
# Keep the old trees in $TMP until the new ones are in place; the EXIT trap deletes them.
if ! { { [ ! -e "$WS_DIR/install" ] || mv "$WS_DIR/install" "$TMP/old/install"; } &&
       { [ ! -e "$WS_DIR/$UI_BUILD" ] || mv "$WS_DIR/$UI_BUILD" "$TMP/old/ui-build"; } &&
       mv "$TMP/stage/install" "$WS_DIR/install" &&
       mv "$TMP/stage/$UI_BUILD" "$WS_DIR/$UI_BUILD"; }; then
    # Keep $TMP (and the old trees in it) on disk until every restore has succeeded.
    trap - EXIT
    had_old="$(ls -A "$TMP/old")"
    failed=0
    # Only a tree whose old copy was moved aside is touched; what is left at its path is a partial new tree.
    restore install "$WS_DIR/install" || failed=1
    restore ui-build "$WS_DIR/$UI_BUILD" || failed=1
    if [ "$failed" = 1 ]; then
        echo "Swap failed and restore failed; the previous build was kept in $TMP/old." >&2
        exit 1
    fi
    rm -rf "$TMP"
    if [ -n "$had_old" ]; then
        echo "Swap failed; restored the previous build." >&2
    else
        echo "Swap failed; no previous build to restore." >&2
    fi
    exit 1
fi

echo "Done. Run: source $WS_DIR/install/setup.bash"
