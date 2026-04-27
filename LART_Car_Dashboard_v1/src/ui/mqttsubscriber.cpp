#include "mqttsubscriber.h"

#include <stdio.h>

#if defined(__GNUC__) || defined(__clang__)
#define LART_WEAK __attribute__((weak))
#else
#define LART_WEAK
#endif

#if defined(LART_ENABLE_MQTT_LIBMOSQUITTO) && LART_ENABLE_MQTT_LIBMOSQUITTO
#if defined(__has_include)
#if __has_include(<mosquitto.h>)
#define LART_HAVE_MQTT_LIBMOSQUITTO 1
#include <mosquitto.h>
#endif
#endif
#endif

#if !defined(LART_HAVE_MQTT_LIBMOSQUITTO)
#define LART_HAVE_MQTT_LIBMOSQUITTO 0
#endif

#if LART_HAVE_MQTT_LIBMOSQUITTO

#include <atomic>
#include <cstdlib>
#include <cstring>

namespace {
constexpr const char *TOPIC_NAME = "/vehicle/speed_kph";
constexpr const char *DEFAULT_HOST = "127.0.0.1";
constexpr int DEFAULT_PORT = 1883;

std::atomic<int> g_is_initialized(0);
std::atomic<int> g_has_speed(0);
std::atomic<float> g_latest_speed_kph(0.0f);

mosquitto *g_client = nullptr;
char g_topic[128] = {0};

int parse_port(const char *port_env) {
    if (port_env == nullptr || *port_env == '\0') {
        return DEFAULT_PORT;
    }

    char *end_ptr = nullptr;
    long value = strtol(port_env, &end_ptr, 10);
    if (end_ptr == port_env || *end_ptr != '\0' || value <= 0 || value > 65535) {
        return DEFAULT_PORT;
    }

    return static_cast<int>(value);
}

void on_connect(struct mosquitto *client, void *, int rc) {
    if (rc != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "mqttsubscriber connect failed: %s\n", mosquitto_connack_string(rc));
        return;
    }

    int sub_rc = mosquitto_subscribe(client, nullptr, g_topic, 0);
    if (sub_rc != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "mqttsubscriber subscribe failed for topic %s: %s\n", g_topic, mosquitto_strerror(sub_rc));
    }
}

void on_message(struct mosquitto *, void *, const struct mosquitto_message *message) {
    if (message == nullptr || message->payload == nullptr || message->payloadlen <= 0) {
        return;
    }

    const char *payload_str = static_cast<const char *>(message->payload);
    char *end_ptr = nullptr;
    float speed = strtof(payload_str, &end_ptr);
    if (end_ptr == payload_str) {
        return;
    }

    g_latest_speed_kph.store(speed);
    g_has_speed.store(1);
}
}  // namespace

LART_WEAK int mqttsubscriber_init(void) {
    if (g_is_initialized.load()) {
        return 0;
    }

    const char *host = getenv("LART_MQTT_HOST");
    if (host == nullptr || *host == '\0') {
        host = DEFAULT_HOST;
    }

    const char *topic = getenv("LART_MQTT_TOPIC");
    if (topic == nullptr || *topic == '\0') {
        topic = TOPIC_NAME;
    }

    snprintf(g_topic, sizeof(g_topic), "%s", topic);
    int port = parse_port(getenv("LART_MQTT_PORT"));

    int lib_rc = mosquitto_lib_init();
    if (lib_rc != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "mqttsubscriber lib init failed: %s\n", mosquitto_strerror(lib_rc));
        return 1;
    }

    g_client = mosquitto_new("lart_dashboard_listener", true, nullptr);
    if (g_client == nullptr) {
        fprintf(stderr, "mqttsubscriber client allocation failed\n");
        mosquitto_lib_cleanup();
        return 1;
    }

    mosquitto_connect_callback_set(g_client, on_connect);
    mosquitto_message_callback_set(g_client, on_message);

    int conn_rc = mosquitto_connect_async(g_client, host, port, 30);
    if (conn_rc != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "mqttsubscriber connect failed: %s\n", mosquitto_strerror(conn_rc));
        mosquitto_destroy(g_client);
        g_client = nullptr;
        mosquitto_lib_cleanup();
        return 1;
    }

    int loop_rc = mosquitto_loop_start(g_client);
    if (loop_rc != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "mqttsubscriber loop start failed: %s\n", mosquitto_strerror(loop_rc));
        mosquitto_disconnect(g_client);
        mosquitto_destroy(g_client);
        g_client = nullptr;
        mosquitto_lib_cleanup();
        return 1;
    }

    g_is_initialized.store(1);
    g_has_speed.store(0);
    return 0;
}

LART_WEAK int mqttsubscriber_spin_some(void) {
    return g_is_initialized.load() ? 0 : 1;
}

LART_WEAK int mqttsubscriber_get_latest_speed(float *speed_kph) {
    if (!g_is_initialized.load() || !g_has_speed.load() || speed_kph == nullptr) {
        return 0;
    }

    *speed_kph = g_latest_speed_kph.load();
    return 1;
}

LART_WEAK void mqttsubscriber_fini(void) {
    if (!g_is_initialized.load()) {
        return;
    }

    if (g_client != nullptr) {
        mosquitto_loop_stop(g_client, true);
        mosquitto_disconnect(g_client);
        mosquitto_destroy(g_client);
        g_client = nullptr;
    }

    mosquitto_lib_cleanup();
    g_is_initialized.store(0);
    g_has_speed.store(0);
    g_latest_speed_kph.store(0.0f);
}

#else

LART_WEAK int mqttsubscriber_init(void) {
    fprintf(stderr, "MQTT subscriber disabled: libmosquitto dev library not available at build time.\n");
    return 1;
}

LART_WEAK int mqttsubscriber_spin_some(void) {
    return 1;
}

LART_WEAK int mqttsubscriber_get_latest_speed(float *speed_kph) {
    (void)speed_kph;
    return 0;
}

LART_WEAK void mqttsubscriber_fini(void) {
}

#endif