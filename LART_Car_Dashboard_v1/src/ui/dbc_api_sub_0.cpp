// Auto-generated from DBC files by generate_dbc_api.py. Do not edit.
#include "dbc_api.h"
#if defined(LART_UI_HAVE_RCLCPP) && LART_UI_HAVE_RCLCPP
#if defined(LART_HAVE_LART_MSGS) && LART_HAVE_LART_MSGS
#include <rclcpp/rclcpp.hpp>
#include <mutex>
#include <vector>
extern std::mutex dbc_api_mutex;
#include <lart_msgs/msg/acu.hpp>
#include <lart_msgs/msg/ams_sdc_feedback.hpp>
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
#include <lart_msgs/msg/icd_request.hpp>
#include <lart_msgs/msg/icd_response.hpp>
#include <lart_msgs/msg/icd_result.hpp>
#include <lart_msgs/msg/inv1_ac_dc_current.hpp>
#include <lart_msgs/msg/inv1_erpm_duty_voltage.hpp>
#include <lart_msgs/msg/inv1_foc.hpp>
#include <lart_msgs/msg/inv1_minmaxaccurrent.hpp>
#include <lart_msgs/msg/inv1_minmaxdccurrent.hpp>
#include <lart_msgs/msg/inv1_misc.hpp>
#include <lart_msgs/msg/inv1_setaccurrent.hpp>
#include <lart_msgs/msg/inv1_setbrakecurrent.hpp>
#include <lart_msgs/msg/inv1_setdigoutput.hpp>

void init_dbc_api_subscribers_chunk_0(std::shared_ptr<rclcpp::Node> node, std::vector<rclcpp::SubscriptionBase::SharedPtr>& subs) {
    auto sensor_qos = rclcpp::QoS(10).best_effort();

    subs.push_back(node->create_subscription<lart_msgs::msg::Acu>(
        "/can/acu", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Acu> msg) {
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
    subs.push_back(node->create_subscription<lart_msgs::msg::AmsSdcFeedback>(
        "/pwt/ams_sdc_feedback", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::AmsSdcFeedback> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ams_sdc_feedback.sdc_state = msg->sdc_state;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::AppsAdcRaw>(
        "/pwt/apps_adc_raw", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::AppsAdcRaw> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.apps_adc_raw.apps1_raw = msg->apps1_raw;
                dbc_api.apps_adc_raw.apps2_raw = msg->apps2_raw;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Aqt1>(
        "/can/aqt1", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Aqt1> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt1.bots = msg->bots;
                dbc_api.aqt1.frt_brk_press = msg->frt_brk_press;
                dbc_api.aqt1.res = msg->res;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Aqt2>(
        "/can/aqt2", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Aqt2> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt2.brake_temp = msg->brake_temp;
                dbc_api.aqt2.tire_temp = msg->tire_temp;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Aqt2>(
        "/data/aqt2", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Aqt2> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt2.brake_temp = msg->brake_temp;
                dbc_api.aqt2.tire_temp = msg->tire_temp;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Aqt3>(
        "/can/aqt3", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Aqt3> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt3.brake_temp = msg->brake_temp;
                dbc_api.aqt3.tire_temp = msg->tire_temp;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Aqt3>(
        "/data/aqt3", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Aqt3> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt3.brake_temp = msg->brake_temp;
                dbc_api.aqt3.tire_temp = msg->tire_temp;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Aqt4>(
        "/can/aqt4", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Aqt4> msg) {
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
        "/data/aqt5", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Aqt5> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt5.brake_temp = msg->brake_temp;
                dbc_api.aqt5.tire_temp = msg->tire_temp;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Aqt6>(
        "/data/aqt6", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Aqt6> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt6.brake_temp = msg->brake_temp;
                dbc_api.aqt6.tire_temp = msg->tire_temp;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Aqt7>(
        "/can/aqt7", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Aqt7> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt7.susp_l = msg->susp_l;
                dbc_api.aqt7.susp_r = msg->susp_r;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Aqt7>(
        "/data/aqt7", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Aqt7> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt7.susp_l = msg->susp_l;
                dbc_api.aqt7.susp_r = msg->susp_r;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Aqt8>(
        "/data/aqt8", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Aqt8> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt8.ntc1 = msg->ntc1;
                dbc_api.aqt8.ntc2 = msg->ntc2;
                dbc_api.aqt8.ntc3 = msg->ntc3;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::AsfSignals>(
        "/can/asf_signals", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::AsfSignals> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.asf_signals.brake_pressure_front = msg->brake_pressure_front;
                dbc_api.asf_signals.brake_pressure_rear = msg->brake_pressure_rear;
                dbc_api.asf_signals.ebs_pressure_tank_front = msg->ebs_pressure_tank_front;
                dbc_api.asf_signals.ebs_pressure_tank_rear = msg->ebs_pressure_tank_rear;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::CubemarsFeedback>(
        "/can/cubemars_feedback", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::CubemarsFeedback> msg) {
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
        "/can/cubemars_position_loop", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::CubemarsPositionLoop> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.cubemars_position_loop.position = msg->position;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Dashboard>(
        "/pwt/dashboard", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Dashboard> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.dashboard.ignition_switch_raw = msg->ignition_switch_raw;
                dbc_api.dashboard.r2d_button_raw = msg->r2d_button_raw;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::DvDynamics1>(
        "/can/dv_dynamics_1", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::DvDynamics1> msg) {
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
        "/can/dv_dynamics_2", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::DvDynamics2> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.dv_dynamics_2.acceleration_lateral = msg->acceleration_lateral;
                dbc_api.dv_dynamics_2.acceleration_longitudinal = msg->acceleration_longitudinal;
                dbc_api.dv_dynamics_2.yaw_rate = msg->yaw_rate;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::DvStatus>(
        "/can/dv_status", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::DvStatus> msg) {
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
    subs.push_back(node->create_subscription<lart_msgs::msg::IcdRequest>(
        "/data/icd_request", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::IcdRequest> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.icd_request.req_accessmode = msg->req_accessmode;
                dbc_api.icd_request.req_accesspw1 = msg->req_accesspw1;
                dbc_api.icd_request.req_accesspw2 = msg->req_accesspw2;
                dbc_api.icd_request.req_accesspw3 = msg->req_accesspw3;
                dbc_api.icd_request.req_accesspw4 = msg->req_accesspw4;
                dbc_api.icd_request.req_accesspw5 = msg->req_accesspw5;
                dbc_api.icd_request.req_accesspw6 = msg->req_accesspw6;
                dbc_api.icd_request.req_averaging = msg->req_averaging;
                dbc_api.icd_request.req_canbaudrate = msg->req_canbaudrate;
                dbc_api.icd_request.req_canidreq_id = msg->req_canidreq_id;
                dbc_api.icd_request.req_canidreq_sn = msg->req_canidreq_sn;
                dbc_api.icd_request.req_canidres_id = msg->req_canidres_id;
                dbc_api.icd_request.req_canidres_sn = msg->req_canidres_sn;
                dbc_api.icd_request.req_canidresp_id = msg->req_canidresp_id;
                dbc_api.icd_request.req_canidresp_sn = msg->req_canidresp_sn;
                dbc_api.icd_request.req_canmode = msg->req_canmode;
                dbc_api.icd_request.req_muxid = msg->req_muxid;
                dbc_api.icd_request.req_newpw1 = msg->req_newpw1;
                dbc_api.icd_request.req_newpw2 = msg->req_newpw2;
                dbc_api.icd_request.req_newpw3 = msg->req_newpw3;
                dbc_api.icd_request.req_newpw4 = msg->req_newpw4;
                dbc_api.icd_request.req_newpw5 = msg->req_newpw5;
                dbc_api.icd_request.req_newpw6 = msg->req_newpw6;
                dbc_api.icd_request.req_oclimitneg = msg->req_oclimitneg;
                dbc_api.icd_request.req_oclimitpos = msg->req_oclimitpos;
                dbc_api.icd_request.req_opinvertcurrent = msg->req_opinvertcurrent;
                dbc_api.icd_request.req_opmode = msg->req_opmode;
                dbc_api.icd_request.req_opoutputrate = msg->req_opoutputrate;
                dbc_api.icd_request.req_resetoption = msg->req_resetoption;
                dbc_api.icd_request.req_sleepahthreshold = msg->req_sleepahthreshold;
                dbc_api.icd_request.req_sleepithreshold = msg->req_sleepithreshold;
                dbc_api.icd_request.req_sleeptimer = msg->req_sleeptimer;
                dbc_api.icd_request.req_wakeahthreshold = msg->req_wakeahthreshold;
                dbc_api.icd_request.req_wakeithreshold = msg->req_wakeithreshold;
                dbc_api.icd_request.req_waketimer = msg->req_waketimer;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::IcdResponse>(
        "/data/icd_response", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::IcdResponse> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.icd_response.resp_accessmode = msg->resp_accessmode;
                dbc_api.icd_response.resp_ahcounter = msg->resp_ahcounter;
                dbc_api.icd_response.resp_articlenumber = msg->resp_articlenumber;
                dbc_api.icd_response.resp_averaging = msg->resp_averaging;
                dbc_api.icd_response.resp_canbaudrate = msg->resp_canbaudrate;
                dbc_api.icd_response.resp_canidreq_id = msg->resp_canidreq_id;
                dbc_api.icd_response.resp_canidreq_sn = msg->resp_canidreq_sn;
                dbc_api.icd_response.resp_canidres_id = msg->resp_canidres_id;
                dbc_api.icd_response.resp_canidres_sn = msg->resp_canidres_sn;
                dbc_api.icd_response.resp_canidresp_id = msg->resp_canidresp_id;
                dbc_api.icd_response.resp_canidresp_sn = msg->resp_canidresp_sn;
                dbc_api.icd_response.resp_canmode = msg->resp_canmode;
                dbc_api.icd_response.resp_devid_char1 = msg->resp_devid_char1;
                dbc_api.icd_response.resp_devid_char2 = msg->resp_devid_char2;
                dbc_api.icd_response.resp_devid_char3 = msg->resp_devid_char3;
                dbc_api.icd_response.resp_devid_char4 = msg->resp_devid_char4;
                dbc_api.icd_response.resp_devid_char5 = msg->resp_devid_char5;
                dbc_api.icd_response.resp_devid_frame = msg->resp_devid_frame;
                dbc_api.icd_response.resp_evtmeascount = msg->resp_evtmeascount;
                dbc_api.icd_response.resp_evtmeastype = msg->resp_evtmeastype;
                dbc_api.icd_response.resp_evtsyscount = msg->resp_evtsyscount;
                dbc_api.icd_response.resp_evtsystype = msg->resp_evtsystype;
                dbc_api.icd_response.resp_fwmajor = msg->resp_fwmajor;
                dbc_api.icd_response.resp_fwminor = msg->resp_fwminor;
                dbc_api.icd_response.resp_fwrelease = msg->resp_fwrelease;
                dbc_api.icd_response.resp_hwver_char1 = msg->resp_hwver_char1;
                dbc_api.icd_response.resp_hwver_char2 = msg->resp_hwver_char2;
                dbc_api.icd_response.resp_hwver_char3 = msg->resp_hwver_char3;
                dbc_api.icd_response.resp_hwver_char4 = msg->resp_hwver_char4;
                dbc_api.icd_response.resp_hwver_char5 = msg->resp_hwver_char5;
                dbc_api.icd_response.resp_hwver_char6 = msg->resp_hwver_char6;
                dbc_api.icd_response.resp_hwver_frame = msg->resp_hwver_frame;
                dbc_api.icd_response.resp_lifetime = msg->resp_lifetime;
                dbc_api.icd_response.resp_ms_adcinterrupt = msg->resp_ms_adcinterrupt;
                dbc_api.icd_response.resp_ms_calibrationdata = msg->resp_ms_calibrationdata;
                dbc_api.icd_response.resp_ms_ocsactive = msg->resp_ms_ocsactive;
                dbc_api.icd_response.resp_ms_opencircuiti = msg->resp_ms_opencircuiti;
                dbc_api.icd_response.resp_ms_opencircuittchip = msg->resp_ms_opencircuittchip;
                dbc_api.icd_response.resp_ms_opencircuittext = msg->resp_ms_opencircuittext;
                dbc_api.icd_response.resp_ms_opencircuitu = msg->resp_ms_opencircuitu;
                dbc_api.icd_response.resp_ms_overflowadcch1 = msg->resp_ms_overflowadcch1;
                dbc_api.icd_response.resp_ms_overflowadcch2 = msg->resp_ms_overflowadcch2;
                dbc_api.icd_response.resp_ms_overflowadcch3 = msg->resp_ms_overflowadcch3;
                dbc_api.icd_response.resp_muxid = msg->resp_muxid;
                dbc_api.icd_response.resp_oclimitneg = msg->resp_oclimitneg;
                dbc_api.icd_response.resp_oclimitpos = msg->resp_oclimitpos;
                dbc_api.icd_response.resp_opinvertcurrent = msg->resp_opinvertcurrent;
                dbc_api.icd_response.resp_opmode = msg->resp_opmode;
                dbc_api.icd_response.resp_opoutputrate = msg->resp_opoutputrate;
                dbc_api.icd_response.resp_pw1 = msg->resp_pw1;
                dbc_api.icd_response.resp_pw2 = msg->resp_pw2;
                dbc_api.icd_response.resp_pw3 = msg->resp_pw3;
                dbc_api.icd_response.resp_pw4 = msg->resp_pw4;
                dbc_api.icd_response.resp_pw5 = msg->resp_pw5;
                dbc_api.icd_response.resp_pw6 = msg->resp_pw6;
                dbc_api.icd_response.resp_resetoption = msg->resp_resetoption;
                dbc_api.icd_response.resp_rev_digit1 = msg->resp_rev_digit1;
                dbc_api.icd_response.resp_rev_digit2 = msg->resp_rev_digit2;
                dbc_api.icd_response.resp_rev_digit3 = msg->resp_rev_digit3;
                dbc_api.icd_response.resp_rev_digit4 = msg->resp_rev_digit4;
                dbc_api.icd_response.resp_serialnumber = msg->resp_serialnumber;
                dbc_api.icd_response.resp_sleepahthreshold = msg->resp_sleepahthreshold;
                dbc_api.icd_response.resp_sleepithreshold = msg->resp_sleepithreshold;
                dbc_api.icd_response.resp_sleeptimer = msg->resp_sleeptimer;
                dbc_api.icd_response.resp_ss_caldatacrc = msg->resp_ss_caldatacrc;
                dbc_api.icd_response.resp_ss_canrx = msg->resp_ss_canrx;
                dbc_api.icd_response.resp_ss_cantx = msg->resp_ss_cantx;
                dbc_api.icd_response.resp_ss_clockmonreset = msg->resp_ss_clockmonreset;
                dbc_api.icd_response.resp_ss_codecrc = msg->resp_ss_codecrc;
                dbc_api.icd_response.resp_ss_configuration = msg->resp_ss_configuration;
                dbc_api.icd_response.resp_ss_copwdreset = msg->resp_ss_copwdreset;
                dbc_api.icd_response.resp_ss_eepromrw = msg->resp_ss_eepromrw;
                dbc_api.icd_response.resp_ss_extpinreset = msg->resp_ss_extpinreset;
                dbc_api.icd_response.resp_ss_lowvoltagereset = msg->resp_ss_lowvoltagereset;
                dbc_api.icd_response.resp_ss_overtemp = msg->resp_ss_overtemp;
                dbc_api.icd_response.resp_ss_powerfailure = msg->resp_ss_powerfailure;
                dbc_api.icd_response.resp_ss_poweronreset = msg->resp_ss_poweronreset;
                dbc_api.icd_response.resp_ss_systemclock = msg->resp_ss_systemclock;
                dbc_api.icd_response.resp_ss_systeminit = msg->resp_ss_systeminit;
                dbc_api.icd_response.resp_ss_undertemp = msg->resp_ss_undertemp;
                dbc_api.icd_response.resp_tchip = msg->resp_tchip;
                dbc_api.icd_response.resp_text = msg->resp_text;
                dbc_api.icd_response.resp_uptime = msg->resp_uptime;
                dbc_api.icd_response.resp_wakeahthreshold = msg->resp_wakeahthreshold;
                dbc_api.icd_response.resp_wakeithreshold = msg->resp_wakeithreshold;
                dbc_api.icd_response.resp_waketimer = msg->resp_waketimer;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::IcdResult>(
        "/data/icd_result", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::IcdResult> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.icd_result.icd_current = msg->icd_current;
                dbc_api.icd_result.icd_msgcounter = msg->icd_msgcounter;
                dbc_api.icd_result.icd_status_measerror = msg->icd_status_measerror;
                dbc_api.icd_result.icd_status_ocs = msg->icd_status_ocs;
                dbc_api.icd_result.icd_status_syserror = msg->icd_status_syserror;
                dbc_api.icd_result.icd_ubat = msg->icd_ubat;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv1AcDcCurrent>(
        "/pwt/inv1_ac_dc_current", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv1AcDcCurrent> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv1_ac_dc_current.inv1_actual_accurrent = msg->inv1_actual_accurrent;
                dbc_api.inv1_ac_dc_current.inv1_actual_dccurrent = msg->inv1_actual_dccurrent;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv1ErpmDutyVoltage>(
        "/pwt/inv1_erpm_duty_voltage", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv1ErpmDutyVoltage> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv1_erpm_duty_voltage.inv1_actual_duty = msg->inv1_actual_duty;
                dbc_api.inv1_erpm_duty_voltage.inv1_actual_erpm = msg->inv1_actual_erpm;
                dbc_api.inv1_erpm_duty_voltage.inv1_actual_inputvoltage = msg->inv1_actual_inputvoltage;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv1Foc>(
        "/pwt/inv1_foc", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv1Foc> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv1_foc.inv1_actual_foc_id = msg->inv1_actual_foc_id;
                dbc_api.inv1_foc.inv1_actual_foc_iq = msg->inv1_actual_foc_iq;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv1Minmaxaccurrent>(
        "/pwt/inv1_minmaxaccurrent", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv1Minmaxaccurrent> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv1_minmaxaccurrent.inv1_availablemaxaccurrent = msg->inv1_availablemaxaccurrent;
                dbc_api.inv1_minmaxaccurrent.inv1_availableminaccurrent = msg->inv1_availableminaccurrent;
                dbc_api.inv1_minmaxaccurrent.inv1_maxaccurrent = msg->inv1_maxaccurrent;
                dbc_api.inv1_minmaxaccurrent.inv1_minaccurrent = msg->inv1_minaccurrent;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv1Minmaxdccurrent>(
        "/pwt/inv1_minmaxdccurrent", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv1Minmaxdccurrent> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv1_minmaxdccurrent.inv1_availablemaxdccurrent = msg->inv1_availablemaxdccurrent;
                dbc_api.inv1_minmaxdccurrent.inv1_availablemindccurrent = msg->inv1_availablemindccurrent;
                dbc_api.inv1_minmaxdccurrent.inv1_maxdccurrent = msg->inv1_maxdccurrent;
                dbc_api.inv1_minmaxdccurrent.inv1_mindccurrent = msg->inv1_mindccurrent;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv1Misc>(
        "/pwt/inv1_misc", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv1Misc> msg) {
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
        "/pwt/inv1_setaccurrent", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv1Setaccurrent> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv1_setaccurrent.inv1_cmd_targetaccurrent = msg->inv1_cmd_targetaccurrent;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv1Setbrakecurrent>(
        "/pwt/inv1_setbrakecurrent", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv1Setbrakecurrent> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv1_setbrakecurrent.inv1_cmd_targetbrakecurrent = msg->inv1_cmd_targetbrakecurrent;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv1Setdigoutput>(
        "/pwt/inv1_setdigoutput", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv1Setdigoutput> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv1_setdigoutput.inv1_cmd_setdigoutput1 = msg->inv1_cmd_setdigoutput1;
                dbc_api.inv1_setdigoutput.inv1_cmd_setdigoutput2 = msg->inv1_cmd_setdigoutput2;
                dbc_api.inv1_setdigoutput.inv1_cmd_setdigoutput3 = msg->inv1_cmd_setdigoutput3;
                dbc_api.inv1_setdigoutput.inv1_cmd_setdigoutput4 = msg->inv1_cmd_setdigoutput4;
            }
        }));
}
#endif
#endif
