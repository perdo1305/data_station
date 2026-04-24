cmake_minimum_required(VERSION 3.8)
project(c_subscriber_example C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Check if building in ROS 2 environment
find_package(ament_cmake QUIET)

if(ament_cmake_FOUND)
    # Try to find ROS 2 C++ packages (without REQUIRED - they're optional)
    find_package(rclcpp QUIET)
    find_package(std_msgs QUIET)

    if(rclcpp_FOUND AND std_msgs_FOUND)
        # Full ROS 2 build possible
        message(STATUS "✓ ROS 2 C++ headers found - enabling ros2subscriber")
        add_executable(speed_subscriber ros2subscriber.cpp)
        ament_target_dependencies(speed_subscriber rclcpp std_msgs)

        install(TARGETS
          speed_subscriber
          DESTINATION lib/${PROJECT_NAME})

        ament_package()
    else()
        # ament_cmake found but ROS 2 development packages missing
        message(STATUS "⚠️  ROS 2 C++ headers not available")
        message(STATUS "    Install with: sudo apt install ros-jazzy-rclcpp ros-jazzy-std-msgs")
        message(STATUS "    Then rebuild: colcon build")
        
        add_executable(speed_subscriber ros2subscriber.cpp)
        ament_package()
    endif()
else()
    # Non-ROS 2 build environment (e.g., EEZ Studio)
    message(STATUS "⚠️  Building outside ROS 2 ament environment")
    message(STATUS "    ROS 2 C++ headers will not be available")
    message(STATUS "    Enable ROS 2 support by building within colcon:")
    message(STATUS "    $ source /opt/ros/jazzy/setup.bash && colcon build")
    
    add_executable(speed_subscriber ros2subscriber.cpp)
endif()