cmake_minimum_required(VERSION 3.8)
project(lart_msgs)

find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(builtin_interfaces REQUIRED)

rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/ButtonEvent.msg"
  "msg/EncoderDelta.msg"
  "msg/CanFrame.msg"
  "msg/DashboardState.msg"
  DEPENDENCIES builtin_interfaces
)

ament_package()
