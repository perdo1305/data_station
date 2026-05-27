#ifndef LART_ROS2SUBSCRIBER_H
#define LART_ROS2SUBSCRIBER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    // AMS
    float ams_state;
    float ams_soc;
    float ams_runtime;
    float ams_fans;
    float ams_mcu_vref;
    float ams_mcu_temp;
    float ams_pec_err;
    float ams_fault_cnt;
    float ams_slaves;
    float ams_fw;
    // precharge
    float precharge_state;
    float air_pos;
    float air_min;
    float ctc_charge;
    float ctc_discharge;
    // pack overview
    float v_max;
    float v_min;
    float t_max;
    float t_min;
    // Inverter DTI
    float inv_erpm;
    float inv_duty;
    float inv_vin;
    float inv_fault;
    float inv_ac_curr;
    float inv_dc_curr;
    float inv_temp_ctrl;
    float inv_temp_mot;
    float inv_throttle;
    float inv_brake;
    float inv_foc_id;
    float inv_foc_iq;
    float inv_drive_en;
    // IVT
    float ivt_current;
    float ivt_u1;
    float ivt_u2;
    float ivt_u3;
    float ivt_temp;
    float ivt_power;
    float ivt_energy;
    // Pedals
    float apps1;
    float apps2;
    // Wheels FL FR RL RR
    float spd_fl; float temp_fl; float brk_fl;
    float spd_fr; float temp_fr; float brk_fr;
    float spd_rl; float temp_rl; float brk_rl;
    float spd_rr; float temp_rr; float brk_rr;
    // Rear sensors (AQT7)
    float ntc1;
    float ntc2;
    float ntc3;
    float susp_r;
    float susp_l;
    float rear_ign;
    float rear_r2d;

    // Autonomous: ACU
    float acu_state;
    float acu_as_state;
    float acu_assi_state;
    float acu_mission;
    float acu_asms;
    float acu_ign;
    float acu_emergency;
    float acu_emerg_cause;
    float acu_cpu_temp;
    // Jetson
    float jetson_as_state;
    float jetson_mission;
    float jetson_temp;
    float jetson_cpu;
    float jetson_gpu;
    float jetson_emerg_cause;
    // VCU
    float vcu_ign_man;
    float vcu_ign_auto;
    float vcu_r2d_man;
    float vcu_r2d_auto;
    float vcu_shutdown;
    float vcu_state;
    float vcu_r2d_raw;
    float vcu_ign_raw;
    float vcu_hv;
    float vcu_brkf;
    float vcu_brkr;
    float vcu_rpm_act;
    float vcu_rpm_tgt;
    float vcu_torque_tgt;
    // DV Dynamics
    float dv_spd_act;
    float dv_spd_tgt;
    float dv_str_act;
    float dv_str_tgt;
    float dv_brk_act;
    float dv_brk_tgt;
    float dv_mot_act;
    float dv_mot_tgt;
    float dv_acc_lon;
    float dv_acc_lat;
    float dv_yaw;
    // DV Status
    float dv_as_status;
    float dv_ebs_state;
    float dv_ami_state;
    float dv_steer_state;
    float dv_ebs_red_state;
    // Pressures
    float ebs_tank_f;
    float ebs_tank_r;
    float brk_press_f;
    float brk_press_r;
    // Steering
    float steer_pos;
    float steer_spd;
    float steer_curr;
    float steer_temp;
    float steer_err;
    float steer_pos_tgt;
    // SLAM
    float slam_laps;
    float slam_cones;
    float slam_all;
    // RES
    float res_signal;
    // AQT1
    float aqt1_brkp;
    float aqt1_res;
    float aqt1_bots;
    // AQT2 / AQT3 / AQT4 / AQT7
    float aqt2_whl_ang;
    float aqt3_whl_ang;
    float aqt4_st_ang;
    float aqt4_inertia;
    float aqt4_emer;
    float aqt7_brkp;
} TelemetryData;

int ros2subscriber_init(void);
int ros2subscriber_spin_some(void);
int ros2subscriber_get_latest_speed(float *speed_kph);
int ros2subscriber_get_latest_hv(float *hv_value);
int ros2subscriber_get_screen_change_request(int *screen_id);
int ros2subscriber_get_telemetry(TelemetryData *out);
void ros2subscriber_fini(void);

#ifdef __cplusplus
}
#endif

#endif