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
#include <lart_msgs/msg/aqt2_temperatures1.hpp>
#include <lart_msgs/msg/aqt2_temperatures10.hpp>
#include <lart_msgs/msg/aqt2_temperatures11.hpp>
#include <lart_msgs/msg/aqt2_temperatures12.hpp>
#include <lart_msgs/msg/aqt2_temperatures13.hpp>
#include <lart_msgs/msg/aqt2_temperatures14.hpp>
#include <lart_msgs/msg/aqt2_temperatures15.hpp>
#include <lart_msgs/msg/aqt2_temperatures16.hpp>
#include <lart_msgs/msg/aqt2_temperatures17.hpp>
#include <lart_msgs/msg/aqt2_temperatures18.hpp>
#include <lart_msgs/msg/aqt2_temperatures19.hpp>
#include <lart_msgs/msg/aqt2_temperatures2.hpp>
#include <lart_msgs/msg/aqt2_temperatures20.hpp>
#include <lart_msgs/msg/aqt2_temperatures21.hpp>
#include <lart_msgs/msg/aqt2_temperatures22.hpp>
#include <lart_msgs/msg/aqt2_temperatures23.hpp>
#include <lart_msgs/msg/aqt2_temperatures24.hpp>
#include <lart_msgs/msg/aqt2_temperatures3.hpp>
#include <lart_msgs/msg/aqt2_temperatures4.hpp>
#include <lart_msgs/msg/aqt2_temperatures5.hpp>
#include <lart_msgs/msg/aqt2_temperatures6.hpp>
#include <lart_msgs/msg/aqt2_temperatures7.hpp>
#include <lart_msgs/msg/aqt2_temperatures8.hpp>
#include <lart_msgs/msg/aqt2_temperatures9.hpp>
#include <lart_msgs/msg/aqt4.hpp>

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
                dbc_api.aqt1.throtle_percentage = msg->throtle_percentage;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Aqt1>(
        "/data/aqt1", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Aqt1> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt1.bots = msg->bots;
                dbc_api.aqt1.frt_brk_press = msg->frt_brk_press;
                dbc_api.aqt1.res = msg->res;
                dbc_api.aqt1.throtle_percentage = msg->throtle_percentage;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Aqt2>(
        "/data/aqt2", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Aqt2> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt2.front_left_wheel_rpm = msg->front_left_wheel_rpm;
                dbc_api.aqt2.front_right_wheel_rpm = msg->front_right_wheel_rpm;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Aqt2Temperatures1>(
        "/data/aqt2_temperatures_1", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Aqt2Temperatures1> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt2_temperatures_1.temperature_001 = msg->temperature_001;
                dbc_api.aqt2_temperatures_1.temperature_002 = msg->temperature_002;
                dbc_api.aqt2_temperatures_1.temperature_003 = msg->temperature_003;
                dbc_api.aqt2_temperatures_1.temperature_004 = msg->temperature_004;
                dbc_api.aqt2_temperatures_1.temperature_005 = msg->temperature_005;
                dbc_api.aqt2_temperatures_1.temperature_006 = msg->temperature_006;
                dbc_api.aqt2_temperatures_1.temperature_007 = msg->temperature_007;
                dbc_api.aqt2_temperatures_1.temperature_008 = msg->temperature_008;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Aqt2Temperatures10>(
        "/data/aqt2_temperatures_10", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Aqt2Temperatures10> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt2_temperatures_10.temperature_073 = msg->temperature_073;
                dbc_api.aqt2_temperatures_10.temperature_074 = msg->temperature_074;
                dbc_api.aqt2_temperatures_10.temperature_075 = msg->temperature_075;
                dbc_api.aqt2_temperatures_10.temperature_076 = msg->temperature_076;
                dbc_api.aqt2_temperatures_10.temperature_077 = msg->temperature_077;
                dbc_api.aqt2_temperatures_10.temperature_078 = msg->temperature_078;
                dbc_api.aqt2_temperatures_10.temperature_079 = msg->temperature_079;
                dbc_api.aqt2_temperatures_10.temperature_080 = msg->temperature_080;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Aqt2Temperatures11>(
        "/data/aqt2_temperatures_11", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Aqt2Temperatures11> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt2_temperatures_11.temperature_081 = msg->temperature_081;
                dbc_api.aqt2_temperatures_11.temperature_082 = msg->temperature_082;
                dbc_api.aqt2_temperatures_11.temperature_083 = msg->temperature_083;
                dbc_api.aqt2_temperatures_11.temperature_084 = msg->temperature_084;
                dbc_api.aqt2_temperatures_11.temperature_085 = msg->temperature_085;
                dbc_api.aqt2_temperatures_11.temperature_086 = msg->temperature_086;
                dbc_api.aqt2_temperatures_11.temperature_087 = msg->temperature_087;
                dbc_api.aqt2_temperatures_11.temperature_088 = msg->temperature_088;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Aqt2Temperatures12>(
        "/data/aqt2_temperatures_12", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Aqt2Temperatures12> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt2_temperatures_12.temperature_089 = msg->temperature_089;
                dbc_api.aqt2_temperatures_12.temperature_090 = msg->temperature_090;
                dbc_api.aqt2_temperatures_12.temperature_091 = msg->temperature_091;
                dbc_api.aqt2_temperatures_12.temperature_092 = msg->temperature_092;
                dbc_api.aqt2_temperatures_12.temperature_093 = msg->temperature_093;
                dbc_api.aqt2_temperatures_12.temperature_094 = msg->temperature_094;
                dbc_api.aqt2_temperatures_12.temperature_095 = msg->temperature_095;
                dbc_api.aqt2_temperatures_12.temperature_096 = msg->temperature_096;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Aqt2Temperatures13>(
        "/data/aqt2_temperatures_13", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Aqt2Temperatures13> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt2_temperatures_13.temperature_097 = msg->temperature_097;
                dbc_api.aqt2_temperatures_13.temperature_098 = msg->temperature_098;
                dbc_api.aqt2_temperatures_13.temperature_099 = msg->temperature_099;
                dbc_api.aqt2_temperatures_13.temperature_100 = msg->temperature_100;
                dbc_api.aqt2_temperatures_13.temperature_101 = msg->temperature_101;
                dbc_api.aqt2_temperatures_13.temperature_102 = msg->temperature_102;
                dbc_api.aqt2_temperatures_13.temperature_103 = msg->temperature_103;
                dbc_api.aqt2_temperatures_13.temperature_104 = msg->temperature_104;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Aqt2Temperatures14>(
        "/data/aqt2_temperatures_14", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Aqt2Temperatures14> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt2_temperatures_14.temperature_105 = msg->temperature_105;
                dbc_api.aqt2_temperatures_14.temperature_106 = msg->temperature_106;
                dbc_api.aqt2_temperatures_14.temperature_107 = msg->temperature_107;
                dbc_api.aqt2_temperatures_14.temperature_108 = msg->temperature_108;
                dbc_api.aqt2_temperatures_14.temperature_109 = msg->temperature_109;
                dbc_api.aqt2_temperatures_14.temperature_110 = msg->temperature_110;
                dbc_api.aqt2_temperatures_14.temperature_111 = msg->temperature_111;
                dbc_api.aqt2_temperatures_14.temperature_112 = msg->temperature_112;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Aqt2Temperatures15>(
        "/data/aqt2_temperatures_15", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Aqt2Temperatures15> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt2_temperatures_15.temperature_113 = msg->temperature_113;
                dbc_api.aqt2_temperatures_15.temperature_114 = msg->temperature_114;
                dbc_api.aqt2_temperatures_15.temperature_115 = msg->temperature_115;
                dbc_api.aqt2_temperatures_15.temperature_116 = msg->temperature_116;
                dbc_api.aqt2_temperatures_15.temperature_117 = msg->temperature_117;
                dbc_api.aqt2_temperatures_15.temperature_118 = msg->temperature_118;
                dbc_api.aqt2_temperatures_15.temperature_119 = msg->temperature_119;
                dbc_api.aqt2_temperatures_15.temperature_120 = msg->temperature_120;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Aqt2Temperatures16>(
        "/data/aqt2_temperatures_16", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Aqt2Temperatures16> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt2_temperatures_16.temperature_121 = msg->temperature_121;
                dbc_api.aqt2_temperatures_16.temperature_122 = msg->temperature_122;
                dbc_api.aqt2_temperatures_16.temperature_123 = msg->temperature_123;
                dbc_api.aqt2_temperatures_16.temperature_124 = msg->temperature_124;
                dbc_api.aqt2_temperatures_16.temperature_125 = msg->temperature_125;
                dbc_api.aqt2_temperatures_16.temperature_126 = msg->temperature_126;
                dbc_api.aqt2_temperatures_16.temperature_127 = msg->temperature_127;
                dbc_api.aqt2_temperatures_16.temperature_128 = msg->temperature_128;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Aqt2Temperatures17>(
        "/data/aqt2_temperatures_17", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Aqt2Temperatures17> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt2_temperatures_17.temperature_129 = msg->temperature_129;
                dbc_api.aqt2_temperatures_17.temperature_130 = msg->temperature_130;
                dbc_api.aqt2_temperatures_17.temperature_131 = msg->temperature_131;
                dbc_api.aqt2_temperatures_17.temperature_132 = msg->temperature_132;
                dbc_api.aqt2_temperatures_17.temperature_133 = msg->temperature_133;
                dbc_api.aqt2_temperatures_17.temperature_134 = msg->temperature_134;
                dbc_api.aqt2_temperatures_17.temperature_135 = msg->temperature_135;
                dbc_api.aqt2_temperatures_17.temperature_136 = msg->temperature_136;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Aqt2Temperatures18>(
        "/data/aqt2_temperatures_18", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Aqt2Temperatures18> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt2_temperatures_18.temperature_137 = msg->temperature_137;
                dbc_api.aqt2_temperatures_18.temperature_138 = msg->temperature_138;
                dbc_api.aqt2_temperatures_18.temperature_139 = msg->temperature_139;
                dbc_api.aqt2_temperatures_18.temperature_140 = msg->temperature_140;
                dbc_api.aqt2_temperatures_18.temperature_141 = msg->temperature_141;
                dbc_api.aqt2_temperatures_18.temperature_142 = msg->temperature_142;
                dbc_api.aqt2_temperatures_18.temperature_143 = msg->temperature_143;
                dbc_api.aqt2_temperatures_18.temperature_144 = msg->temperature_144;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Aqt2Temperatures19>(
        "/data/aqt2_temperatures_19", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Aqt2Temperatures19> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt2_temperatures_19.temperature_145 = msg->temperature_145;
                dbc_api.aqt2_temperatures_19.temperature_146 = msg->temperature_146;
                dbc_api.aqt2_temperatures_19.temperature_147 = msg->temperature_147;
                dbc_api.aqt2_temperatures_19.temperature_148 = msg->temperature_148;
                dbc_api.aqt2_temperatures_19.temperature_149 = msg->temperature_149;
                dbc_api.aqt2_temperatures_19.temperature_150 = msg->temperature_150;
                dbc_api.aqt2_temperatures_19.temperature_151 = msg->temperature_151;
                dbc_api.aqt2_temperatures_19.temperature_152 = msg->temperature_152;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Aqt2Temperatures2>(
        "/data/aqt2_temperatures_2", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Aqt2Temperatures2> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt2_temperatures_2.temperature_009 = msg->temperature_009;
                dbc_api.aqt2_temperatures_2.temperature_010 = msg->temperature_010;
                dbc_api.aqt2_temperatures_2.temperature_011 = msg->temperature_011;
                dbc_api.aqt2_temperatures_2.temperature_012 = msg->temperature_012;
                dbc_api.aqt2_temperatures_2.temperature_013 = msg->temperature_013;
                dbc_api.aqt2_temperatures_2.temperature_014 = msg->temperature_014;
                dbc_api.aqt2_temperatures_2.temperature_015 = msg->temperature_015;
                dbc_api.aqt2_temperatures_2.temperature_016 = msg->temperature_016;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Aqt2Temperatures20>(
        "/data/aqt2_temperatures_20", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Aqt2Temperatures20> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt2_temperatures_20.temperature_153 = msg->temperature_153;
                dbc_api.aqt2_temperatures_20.temperature_154 = msg->temperature_154;
                dbc_api.aqt2_temperatures_20.temperature_155 = msg->temperature_155;
                dbc_api.aqt2_temperatures_20.temperature_156 = msg->temperature_156;
                dbc_api.aqt2_temperatures_20.temperature_157 = msg->temperature_157;
                dbc_api.aqt2_temperatures_20.temperature_158 = msg->temperature_158;
                dbc_api.aqt2_temperatures_20.temperature_159 = msg->temperature_159;
                dbc_api.aqt2_temperatures_20.temperature_160 = msg->temperature_160;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Aqt2Temperatures21>(
        "/data/aqt2_temperatures_21", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Aqt2Temperatures21> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt2_temperatures_21.temperature_161 = msg->temperature_161;
                dbc_api.aqt2_temperatures_21.temperature_162 = msg->temperature_162;
                dbc_api.aqt2_temperatures_21.temperature_163 = msg->temperature_163;
                dbc_api.aqt2_temperatures_21.temperature_164 = msg->temperature_164;
                dbc_api.aqt2_temperatures_21.temperature_165 = msg->temperature_165;
                dbc_api.aqt2_temperatures_21.temperature_166 = msg->temperature_166;
                dbc_api.aqt2_temperatures_21.temperature_167 = msg->temperature_167;
                dbc_api.aqt2_temperatures_21.temperature_168 = msg->temperature_168;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Aqt2Temperatures22>(
        "/data/aqt2_temperatures_22", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Aqt2Temperatures22> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt2_temperatures_22.temperature_169 = msg->temperature_169;
                dbc_api.aqt2_temperatures_22.temperature_170 = msg->temperature_170;
                dbc_api.aqt2_temperatures_22.temperature_171 = msg->temperature_171;
                dbc_api.aqt2_temperatures_22.temperature_172 = msg->temperature_172;
                dbc_api.aqt2_temperatures_22.temperature_173 = msg->temperature_173;
                dbc_api.aqt2_temperatures_22.temperature_174 = msg->temperature_174;
                dbc_api.aqt2_temperatures_22.temperature_175 = msg->temperature_175;
                dbc_api.aqt2_temperatures_22.temperature_176 = msg->temperature_176;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Aqt2Temperatures23>(
        "/data/aqt2_temperatures_23", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Aqt2Temperatures23> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt2_temperatures_23.temperature_177 = msg->temperature_177;
                dbc_api.aqt2_temperatures_23.temperature_178 = msg->temperature_178;
                dbc_api.aqt2_temperatures_23.temperature_179 = msg->temperature_179;
                dbc_api.aqt2_temperatures_23.temperature_180 = msg->temperature_180;
                dbc_api.aqt2_temperatures_23.temperature_181 = msg->temperature_181;
                dbc_api.aqt2_temperatures_23.temperature_182 = msg->temperature_182;
                dbc_api.aqt2_temperatures_23.temperature_183 = msg->temperature_183;
                dbc_api.aqt2_temperatures_23.temperature_184 = msg->temperature_184;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Aqt2Temperatures24>(
        "/data/aqt2_temperatures_24", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Aqt2Temperatures24> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt2_temperatures_24.temperature_185 = msg->temperature_185;
                dbc_api.aqt2_temperatures_24.temperature_186 = msg->temperature_186;
                dbc_api.aqt2_temperatures_24.temperature_187 = msg->temperature_187;
                dbc_api.aqt2_temperatures_24.temperature_188 = msg->temperature_188;
                dbc_api.aqt2_temperatures_24.temperature_189 = msg->temperature_189;
                dbc_api.aqt2_temperatures_24.temperature_190 = msg->temperature_190;
                dbc_api.aqt2_temperatures_24.temperature_191 = msg->temperature_191;
                dbc_api.aqt2_temperatures_24.temperature_192 = msg->temperature_192;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Aqt2Temperatures3>(
        "/data/aqt2_temperatures_3", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Aqt2Temperatures3> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt2_temperatures_3.temperature_017 = msg->temperature_017;
                dbc_api.aqt2_temperatures_3.temperature_018 = msg->temperature_018;
                dbc_api.aqt2_temperatures_3.temperature_019 = msg->temperature_019;
                dbc_api.aqt2_temperatures_3.temperature_020 = msg->temperature_020;
                dbc_api.aqt2_temperatures_3.temperature_021 = msg->temperature_021;
                dbc_api.aqt2_temperatures_3.temperature_022 = msg->temperature_022;
                dbc_api.aqt2_temperatures_3.temperature_023 = msg->temperature_023;
                dbc_api.aqt2_temperatures_3.temperature_024 = msg->temperature_024;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Aqt2Temperatures4>(
        "/data/aqt2_temperatures_4", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Aqt2Temperatures4> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt2_temperatures_4.temperature_025 = msg->temperature_025;
                dbc_api.aqt2_temperatures_4.temperature_026 = msg->temperature_026;
                dbc_api.aqt2_temperatures_4.temperature_027 = msg->temperature_027;
                dbc_api.aqt2_temperatures_4.temperature_028 = msg->temperature_028;
                dbc_api.aqt2_temperatures_4.temperature_029 = msg->temperature_029;
                dbc_api.aqt2_temperatures_4.temperature_030 = msg->temperature_030;
                dbc_api.aqt2_temperatures_4.temperature_031 = msg->temperature_031;
                dbc_api.aqt2_temperatures_4.temperature_032 = msg->temperature_032;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Aqt2Temperatures5>(
        "/data/aqt2_temperatures_5", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Aqt2Temperatures5> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt2_temperatures_5.temperature_033 = msg->temperature_033;
                dbc_api.aqt2_temperatures_5.temperature_034 = msg->temperature_034;
                dbc_api.aqt2_temperatures_5.temperature_035 = msg->temperature_035;
                dbc_api.aqt2_temperatures_5.temperature_036 = msg->temperature_036;
                dbc_api.aqt2_temperatures_5.temperature_037 = msg->temperature_037;
                dbc_api.aqt2_temperatures_5.temperature_038 = msg->temperature_038;
                dbc_api.aqt2_temperatures_5.temperature_039 = msg->temperature_039;
                dbc_api.aqt2_temperatures_5.temperature_040 = msg->temperature_040;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Aqt2Temperatures6>(
        "/data/aqt2_temperatures_6", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Aqt2Temperatures6> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt2_temperatures_6.temperature_041 = msg->temperature_041;
                dbc_api.aqt2_temperatures_6.temperature_042 = msg->temperature_042;
                dbc_api.aqt2_temperatures_6.temperature_043 = msg->temperature_043;
                dbc_api.aqt2_temperatures_6.temperature_044 = msg->temperature_044;
                dbc_api.aqt2_temperatures_6.temperature_045 = msg->temperature_045;
                dbc_api.aqt2_temperatures_6.temperature_046 = msg->temperature_046;
                dbc_api.aqt2_temperatures_6.temperature_047 = msg->temperature_047;
                dbc_api.aqt2_temperatures_6.temperature_048 = msg->temperature_048;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Aqt2Temperatures7>(
        "/data/aqt2_temperatures_7", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Aqt2Temperatures7> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt2_temperatures_7.temperature_049 = msg->temperature_049;
                dbc_api.aqt2_temperatures_7.temperature_050 = msg->temperature_050;
                dbc_api.aqt2_temperatures_7.temperature_051 = msg->temperature_051;
                dbc_api.aqt2_temperatures_7.temperature_052 = msg->temperature_052;
                dbc_api.aqt2_temperatures_7.temperature_053 = msg->temperature_053;
                dbc_api.aqt2_temperatures_7.temperature_054 = msg->temperature_054;
                dbc_api.aqt2_temperatures_7.temperature_055 = msg->temperature_055;
                dbc_api.aqt2_temperatures_7.temperature_056 = msg->temperature_056;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Aqt2Temperatures8>(
        "/data/aqt2_temperatures_8", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Aqt2Temperatures8> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt2_temperatures_8.temperature_057 = msg->temperature_057;
                dbc_api.aqt2_temperatures_8.temperature_058 = msg->temperature_058;
                dbc_api.aqt2_temperatures_8.temperature_059 = msg->temperature_059;
                dbc_api.aqt2_temperatures_8.temperature_060 = msg->temperature_060;
                dbc_api.aqt2_temperatures_8.temperature_061 = msg->temperature_061;
                dbc_api.aqt2_temperatures_8.temperature_062 = msg->temperature_062;
                dbc_api.aqt2_temperatures_8.temperature_063 = msg->temperature_063;
                dbc_api.aqt2_temperatures_8.temperature_064 = msg->temperature_064;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Aqt2Temperatures9>(
        "/data/aqt2_temperatures_9", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Aqt2Temperatures9> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt2_temperatures_9.temperature_065 = msg->temperature_065;
                dbc_api.aqt2_temperatures_9.temperature_066 = msg->temperature_066;
                dbc_api.aqt2_temperatures_9.temperature_067 = msg->temperature_067;
                dbc_api.aqt2_temperatures_9.temperature_068 = msg->temperature_068;
                dbc_api.aqt2_temperatures_9.temperature_069 = msg->temperature_069;
                dbc_api.aqt2_temperatures_9.temperature_070 = msg->temperature_070;
                dbc_api.aqt2_temperatures_9.temperature_071 = msg->temperature_071;
                dbc_api.aqt2_temperatures_9.temperature_072 = msg->temperature_072;
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
}
#endif
#endif
