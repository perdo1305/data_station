#ifndef LART_ROS2SUBSCRIBER_H
#define LART_ROS2SUBSCRIBER_H

#ifdef __cplusplus
extern "C" {
#endif

int ros2subscriber_init(void);
int ros2subscriber_spin_some(void);
int ros2subscriber_get_latest_speed(float *speed_kph);
int ros2subscriber_get_latest_hv(float *hv_value);
int ros2subscriber_get_screen_change_request(int *screen_id);
void ros2subscriber_fini(void);

#ifdef __cplusplus
}
#endif

#endif