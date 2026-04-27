#ifndef LART_MQTTSUBSCRIBER_H
#define LART_MQTTSUBSCRIBER_H

#ifdef __cplusplus
extern "C" {
#endif

int mqttsubscriber_init(void);
int mqttsubscriber_spin_some(void);
int mqttsubscriber_get_latest_speed(float *speed_kph);
void mqttsubscriber_fini(void);

#ifdef __cplusplus
}
#endif

#endif