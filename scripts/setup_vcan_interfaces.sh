#!/bin/bash

set -e

if [ "$#" -eq 0 ]; then
    echo "Usage: $0 <vcan-interface> [...]" >&2
    exit 2
fi

sudo modprobe vcan

for interface in "$@"; do
    if ! ip link show "$interface" >/dev/null 2>&1; then
        echo "Creating virtual CAN interface $interface..."
        sudo ip link add "$interface" type vcan
    fi

    if ! ip link show "$interface" 2>/dev/null | grep -q "UP"; then
        sudo ip link set "$interface" up
    fi
    echo "✓ Virtual CAN interface $interface is up."
done
