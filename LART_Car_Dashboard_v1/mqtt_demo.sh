#!/usr/bin/env bash
# MQTT demo helper for the dashboard speed topic.

set -euo pipefail

TOPIC="${LART_MQTT_TOPIC:-/vehicle/speed_kph}"
HOST="${LART_MQTT_HOST:-127.0.0.1}"
PORT="${LART_MQTT_PORT:-1883}"

usage() {
    cat <<'EOF'
Usage:
  ./mqtt_demo.sh sub
  ./mqtt_demo.sh pub <speed_kph>
  ./mqtt_demo.sh ramp
  ./mqtt_demo.sh random [count]

Environment overrides:
  LART_MQTT_HOST   MQTT broker host (default: 127.0.0.1)
  LART_MQTT_PORT   MQTT broker port (default: 1883)
  LART_MQTT_TOPIC  Topic to use (default: /vehicle/speed_kph)

Examples:
  ./mqtt_demo.sh sub
  ./mqtt_demo.sh pub 42.5
  ./mqtt_demo.sh ramp
  ./mqtt_demo.sh random 20
EOF
}

require_cmd() {
    if ! command -v "$1" >/dev/null 2>&1; then
        echo "Missing required command: $1"
        exit 1
    fi
}

check_broker() {
    if (echo >"/dev/tcp/${HOST}/${PORT}") >/dev/null 2>&1; then
        return 0
    fi

    echo "Cannot connect to MQTT broker at ${HOST}:${PORT}."
    echo "Start one locally, for example:"
    echo "  docker run -d --name lart-mqtt -p 1883:1883 eclipse-mosquitto:2"
    echo "Or point to another broker with:"
    echo "  LART_MQTT_HOST=<host> LART_MQTT_PORT=<port> ./mqtt_demo.sh <command>"
    exit 1
}

check_number() {
    if ! [[ "$1" =~ ^-?[0-9]+([.][0-9]+)?$ ]]; then
        echo "Invalid speed value: $1"
        exit 1
    fi
}

pub_speed() {
    local speed="$1"
    check_number "$speed"
    mosquitto_pub -h "$HOST" -p "$PORT" -t "$TOPIC" -m "$speed"
    echo "Published speed_kph=$speed to $TOPIC (broker $HOST:$PORT)"
}

sub_speed() {
    echo "Subscribing to $TOPIC on $HOST:$PORT (Ctrl+C to stop)"
    mosquitto_sub -h "$HOST" -p "$PORT" -t "$TOPIC" -v
}

run_ramp() {
    echo "Publishing ramp sequence to $TOPIC on $HOST:$PORT"
    for speed in 0 5 10 20 35 50 65 80 95 110 95 80 60 40 20 10 0; do
        pub_speed "$speed"
        sleep 0.35
    done
    echo "Ramp sequence complete"
}

run_random() {
    local count="${1:-10}"
    if ! [[ "$count" =~ ^[0-9]+$ ]] || [[ "$count" -lt 1 ]]; then
        echo "Count must be a positive integer"
        exit 1
    fi

    echo "Publishing $count random speed values to $TOPIC on $HOST:$PORT"
    for ((i = 1; i <= count; i++)); do
        local speed
        speed="$((RANDOM % 121))"
        pub_speed "$speed"
        sleep 0.25
    done
    echo "Random sequence complete"
}

main() {
    local cmd="${1:-}"
    case "$cmd" in
        sub)
            require_cmd mosquitto_sub
            check_broker
            sub_speed
            ;;
        pub)
            require_cmd mosquitto_pub
            check_broker
            if [[ $# -lt 2 ]]; then
                usage
                exit 1
            fi
            pub_speed "$2"
            ;;
        ramp)
            require_cmd mosquitto_pub
            check_broker
            run_ramp
            ;;
        random)
            require_cmd mosquitto_pub
            check_broker
            run_random "${2:-10}"
            ;;
        -h|--help|help|"")
            usage
            ;;
        *)
            echo "Unknown command: $cmd"
            usage
            exit 1
            ;;
    esac
}

main "$@"