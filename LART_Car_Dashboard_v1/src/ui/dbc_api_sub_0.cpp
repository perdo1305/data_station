// Auto-generated from DBC files by generate_dbc_api.py. Do not edit.
#include "dbc_api.h"
#if defined(LART_UI_HAVE_RCLCPP) && LART_UI_HAVE_RCLCPP
#if defined(LART_HAVE_LART_MSGS) && LART_HAVE_LART_MSGS
#include <rclcpp/rclcpp.hpp>
#include <mutex>
#include <vector>
extern std::mutex dbc_api_mutex;
#include <lart_msgs/msg/acu.hpp>
#include <lart_msgs/msg/apps_adc_raw.hpp>
#include <lart_msgs/msg/aqt1.hpp>
#include <lart_msgs/msg/aqt2.hpp>
#include <lart_msgs/msg/aqt3.hpp>
#include <lart_msgs/msg/aqt4.hpp>
#include <lart_msgs/msg/aqt5.hpp>
#include <lart_msgs/msg/aqt6.hpp>
#include <lart_msgs/msg/aqt7.hpp>
#include <lart_msgs/msg/aqt8.hpp>
#include <lart_msgs/msg/asf_signals.hpp>
#include <lart_msgs/msg/cubemars_feedback.hpp>
#include <lart_msgs/msg/cubemars_position_loop.hpp>
#include <lart_msgs/msg/dashboard.hpp>
#include <lart_msgs/msg/dv_dynamics1.hpp>
#include <lart_msgs/msg/dv_dynamics2.hpp>
#include <lart_msgs/msg/dv_status.hpp>
#include <lart_msgs/msg/inv1_ac_dc_current.hpp>
#include <lart_msgs/msg/inv1_erpm_duty_voltage.hpp>
#include <lart_msgs/msg/inv1_foc.hpp>
#include <lart_msgs/msg/inv1_minmaxaccurrent.hpp>
#include <lart_msgs/msg/inv1_minmaxdccurrent.hpp>
#include <lart_msgs/msg/inv1_misc.hpp>
#include <lart_msgs/msg/inv1_setaccurrent.hpp>
#include <lart_msgs/msg/inv1_setbrakecurrent.hpp>
#include <lart_msgs/msg/inv1_setdigoutput.hpp>
#include <lart_msgs/msg/inv1_setdriveenable.hpp>
#include <lart_msgs/msg/inv1_seterpm.hpp>
#include <lart_msgs/msg/inv1_setmaxacbrakecurrent.hpp>
#include <lart_msgs/msg/inv1_setmaxaccurrent.hpp>

void init_dbc_api_subscribers_chunk_0(std::shared_ptr<rclcpp::Node> node, std::vector<rclcpp::SubscriptionBase::SharedPtr>& subs) {
    auto sensor_qos = rclcpp::QoS(10).best_effort();

    subs.push_back(node->create_subscription<lart_msgs::msg::Acu>(
        "/can/dbc/acu", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Acu> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.acu.acu_cpu_temp = msg->acu_cpu_temp;
                dbc_api.acu.acu_state = msg->acu_state;
                dbc_api.acu.as_state = msg->as_state;
                dbc_api.acu.asms = msg->asms;
                dbc_api.acu.assi_state = msg->assi_state;
                dbc_api.acu.emergency = msg->emergency;
                dbc_api.acu.emergency_cause = msg->emergency_cause;
                dbc_api.acu.ign = msg->ign;
                dbc_api.acu.mission_select = msg->mission_select;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::AppsAdcRaw>(
        "/can/dbc/apps_adc_raw", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::AppsAdcRaw> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.apps_adc_raw.apps1_raw = msg->apps1_raw;
                dbc_api.apps_adc_raw.apps2_raw = msg->apps2_raw;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Aqt1>(
        "/can/dbc/aqt1", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Aqt1> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt1.bots = msg->bots;
                dbc_api.aqt1.frt_brk_press = msg->frt_brk_press;
                dbc_api.aqt1.res = msg->res;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Aqt2>(
        "/can/dbc/aqt2", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Aqt2> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt2.brake_temp = msg->brake_temp;
                dbc_api.aqt2.tire_temp = msg->tire_temp;
                dbc_api.aqt2.wheel_spd = msg->wheel_spd;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Aqt3>(
        "/can/dbc/aqt3", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Aqt3> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt3.brake_temp = msg->brake_temp;
                dbc_api.aqt3.tire_temp = msg->tire_temp;
                dbc_api.aqt3.wheel_spd = msg->wheel_spd;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Aqt4>(
        "/can/dbc/aqt4", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Aqt4> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt4.emergency = msg->emergency;
                dbc_api.aqt4.inertia = msg->inertia;
                dbc_api.aqt4.st_angle = msg->st_angle;
                dbc_api.aqt4.susp_l = msg->susp_l;
                dbc_api.aqt4.susp_r = msg->susp_r;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Aqt5>(
        "/can/dbc/aqt5", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Aqt5> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt5.brake_temp = msg->brake_temp;
                dbc_api.aqt5.tire_temp = msg->tire_temp;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Aqt6>(
        "/can/dbc/aqt6", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Aqt6> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt6.brake_temp = msg->brake_temp;
                dbc_api.aqt6.tire_temp = msg->tire_temp;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Aqt7>(
        "/can/dbc/aqt7", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Aqt7> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt7.rear_brk_press = msg->rear_brk_press;
                dbc_api.aqt7.susp_l = msg->susp_l;
                dbc_api.aqt7.susp_r = msg->susp_r;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Aqt8>(
        "/can/dbc/aqt8", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Aqt8> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt8.ntc1 = msg->ntc1;
                dbc_api.aqt8.ntc2 = msg->ntc2;
                dbc_api.aqt8.ntc3 = msg->ntc3;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::AsfSignals>(
        "/can/dbc/asf_signals", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::AsfSignals> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.asf_signals.brake_pressure_front = msg->brake_pressure_front;
                dbc_api.asf_signals.brake_pressure_rear = msg->brake_pressure_rear;
                dbc_api.asf_signals.ebs_pressure_tank_front = msg->ebs_pressure_tank_front;
                dbc_api.asf_signals.ebs_pressure_tank_rear = msg->ebs_pressure_tank_rear;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::CubemarsFeedback>(
        "/can/dbc/cubemars_feedback", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::CubemarsFeedback> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.cubemars_feedback.current = msg->current;
                dbc_api.cubemars_feedback.driver_temp = msg->driver_temp;
                dbc_api.cubemars_feedback.error_code = msg->error_code;
                dbc_api.cubemars_feedback.position = msg->position;
                dbc_api.cubemars_feedback.speed_rpm = msg->speed_rpm;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::CubemarsPositionLoop>(
        "/can/dbc/cubemars_position_loop", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::CubemarsPositionLoop> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.cubemars_position_loop.position = msg->position;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Dashboard>(
        "/can/dbc/dashboard", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Dashboard> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.dashboard.ignition_switch_raw = msg->ignition_switch_raw;
                dbc_api.dashboard.r2d_button_raw = msg->r2d_button_raw;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::DvDynamics1>(
        "/can/dbc/dv_dynamics_1", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::DvDynamics1> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.dv_dynamics_1.brake_hydr_actual = msg->brake_hydr_actual;
                dbc_api.dv_dynamics_1.brake_hydr_target = msg->brake_hydr_target;
                dbc_api.dv_dynamics_1.motor_moment_actual = msg->motor_moment_actual;
                dbc_api.dv_dynamics_1.motor_moment_target = msg->motor_moment_target;
                dbc_api.dv_dynamics_1.speed_actual = msg->speed_actual;
                dbc_api.dv_dynamics_1.speed_target = msg->speed_target;
                dbc_api.dv_dynamics_1.steering_angle_actual = msg->steering_angle_actual;
                dbc_api.dv_dynamics_1.steering_angle_target = msg->steering_angle_target;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::DvDynamics2>(
        "/can/dbc/dv_dynamics_2", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::DvDynamics2> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.dv_dynamics_2.acceleration_lateral = msg->acceleration_lateral;
                dbc_api.dv_dynamics_2.acceleration_longitudinal = msg->acceleration_longitudinal;
                dbc_api.dv_dynamics_2.yaw_rate = msg->yaw_rate;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::DvStatus>(
        "/can/dbc/dv_status", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::DvStatus> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.dv_status.ami_state = msg->ami_state;
                dbc_api.dv_status.as_status = msg->as_status;
                dbc_api.dv_status.asb_ebs_state = msg->asb_ebs_state;
                dbc_api.dv_status.asb_redundancy_state = msg->asb_redundancy_state;
                dbc_api.dv_status.cones_count_actual = msg->cones_count_actual;
                dbc_api.dv_status.cones_count_all = msg->cones_count_all;
                dbc_api.dv_status.lap_counter = msg->lap_counter;
                dbc_api.dv_status.steering_state = msg->steering_state;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv1AcDcCurrent>(
        "/can/dbc/inv1_ac_dc_current", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv1AcDcCurrent> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv1_ac_dc_current.inv1_actual_accurrent = msg->inv1_actual_accurrent;
                dbc_api.inv1_ac_dc_current.inv1_actual_dccurrent = msg->inv1_actual_dccurrent;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv1ErpmDutyVoltage>(
        "/can/dbc/inv1_erpm_duty_voltage", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv1ErpmDutyVoltage> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv1_erpm_duty_voltage.inv1_actual_duty = msg->inv1_actual_duty;
                dbc_api.inv1_erpm_duty_voltage.inv1_actual_erpm = msg->inv1_actual_erpm;
                dbc_api.inv1_erpm_duty_voltage.inv1_actual_inputvoltage = msg->inv1_actual_inputvoltage;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv1Foc>(
        "/can/dbc/inv1_foc", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv1Foc> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv1_foc.inv1_actual_foc_id = msg->inv1_actual_foc_id;
                dbc_api.inv1_foc.inv1_actual_foc_iq = msg->inv1_actual_foc_iq;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv1Minmaxaccurrent>(
        "/can/dbc/inv1_minmaxaccurrent", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv1Minmaxaccurrent> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv1_minmaxaccurrent.inv1_availablemaxaccurrent = msg->inv1_availablemaxaccurrent;
                dbc_api.inv1_minmaxaccurrent.inv1_availableminaccurrent = msg->inv1_availableminaccurrent;
                dbc_api.inv1_minmaxaccurrent.inv1_maxaccurrent = msg->inv1_maxaccurrent;
                dbc_api.inv1_minmaxaccurrent.inv1_minaccurrent = msg->inv1_minaccurrent;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv1Minmaxdccurrent>(
        "/can/dbc/inv1_minmaxdccurrent", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv1Minmaxdccurrent> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv1_minmaxdccurrent.inv1_availablemaxdccurrent = msg->inv1_availablemaxdccurrent;
                dbc_api.inv1_minmaxdccurrent.inv1_availablemindccurrent = msg->inv1_availablemindccurrent;
                dbc_api.inv1_minmaxdccurrent.inv1_maxdccurrent = msg->inv1_maxdccurrent;
                dbc_api.inv1_minmaxdccurrent.inv1_mindccurrent = msg->inv1_mindccurrent;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv1Misc>(
        "/can/dbc/inv1_misc", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv1Misc> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv1_misc.inv1_actual_brake = msg->inv1_actual_brake;
                dbc_api.inv1_misc.inv1_actual_throttle = msg->inv1_actual_throttle;
                dbc_api.inv1_misc.inv1_can_map_version = msg->inv1_can_map_version;
                dbc_api.inv1_misc.inv1_capacitor_temp_limit = msg->inv1_capacitor_temp_limit;
                dbc_api.inv1_misc.inv1_dc_current_limit = msg->inv1_dc_current_limit;
                dbc_api.inv1_misc.inv1_digital_input_1 = msg->inv1_digital_input_1;
                dbc_api.inv1_misc.inv1_digital_input_2 = msg->inv1_digital_input_2;
                dbc_api.inv1_misc.inv1_digital_input_3 = msg->inv1_digital_input_3;
                dbc_api.inv1_misc.inv1_digital_input_4 = msg->inv1_digital_input_4;
                dbc_api.inv1_misc.inv1_digital_output_1 = msg->inv1_digital_output_1;
                dbc_api.inv1_misc.inv1_digital_output_2 = msg->inv1_digital_output_2;
                dbc_api.inv1_misc.inv1_digital_output_3 = msg->inv1_digital_output_3;
                dbc_api.inv1_misc.inv1_digital_output_4 = msg->inv1_digital_output_4;
                dbc_api.inv1_misc.inv1_drive_enable = msg->inv1_drive_enable;
                dbc_api.inv1_misc.inv1_drive_enable_limit = msg->inv1_drive_enable_limit;
                dbc_api.inv1_misc.inv1_igbt_accel_limit = msg->inv1_igbt_accel_limit;
                dbc_api.inv1_misc.inv1_igbt_temp_limit = msg->inv1_igbt_temp_limit;
                dbc_api.inv1_misc.inv1_input_voltage_limit = msg->inv1_input_voltage_limit;
                dbc_api.inv1_misc.inv1_motor_accel_limit = msg->inv1_motor_accel_limit;
                dbc_api.inv1_misc.inv1_motor_temp_limit = msg->inv1_motor_temp_limit;
                dbc_api.inv1_misc.inv1_power_limit = msg->inv1_power_limit;
                dbc_api.inv1_misc.inv1_rpm_max_limit = msg->inv1_rpm_max_limit;
                dbc_api.inv1_misc.inv1_rpm_min_limit = msg->inv1_rpm_min_limit;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv1Setaccurrent>(
        "/can/dbc/inv1_setaccurrent", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv1Setaccurrent> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv1_setaccurrent.inv1_cmd_targetaccurrent = msg->inv1_cmd_targetaccurrent;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv1Setbrakecurrent>(
        "/can/dbc/inv1_setbrakecurrent", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv1Setbrakecurrent> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv1_setbrakecurrent.inv1_cmd_targetbrakecurrent = msg->inv1_cmd_targetbrakecurrent;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv1Setdigoutput>(
        "/can/dbc/inv1_setdigoutput", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv1Setdigoutput> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv1_setdigoutput.inv1_cmd_setdigoutput1 = msg->inv1_cmd_setdigoutput1;
                dbc_api.inv1_setdigoutput.inv1_cmd_setdigoutput2 = msg->inv1_cmd_setdigoutput2;
                dbc_api.inv1_setdigoutput.inv1_cmd_setdigoutput3 = msg->inv1_cmd_setdigoutput3;
                dbc_api.inv1_setdigoutput.inv1_cmd_setdigoutput4 = msg->inv1_cmd_setdigoutput4;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv1Setdriveenable>(
        "/can/dbc/inv1_setdriveenable", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv1Setdriveenable> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv1_setdriveenable.inv1_cmd_driveenable = msg->inv1_cmd_driveenable;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv1Seterpm>(
        "/can/dbc/inv1_seterpm", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv1Seterpm> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv1_seterpm.inv1_cmd_targetspeed = msg->inv1_cmd_targetspeed;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv1Setmaxacbrakecurrent>(
        "/can/dbc/inv1_setmaxacbrakecurrent", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv1Setmaxacbrakecurrent> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv1_setmaxacbrakecurrent.inv1_cmd_maxacbrakecurrent = msg->inv1_cmd_maxacbrakecurrent;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv1Setmaxaccurrent>(
        "/can/dbc/inv1_setmaxaccurrent", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv1Setmaxaccurrent> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv1_setmaxaccurrent.inv1_cmd_maxaccurrent = msg->inv1_cmd_maxaccurrent;
            }
        }));
}
#endif
#endif
