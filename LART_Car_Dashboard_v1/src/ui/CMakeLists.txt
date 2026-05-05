cmake_minimum_required(VERSION 3.8)
project(speed_subscriber LANGUAGES C CXX)

# Compiler flags
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# ROS 2 (required)
find_package(ament_cmake QUIET)
find_package(PkgConfig REQUIRED)
find_package(Threads REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

# Optional: subscribe to /vehicle/dashboard_state if lart_msgs is available
find_package(lart_msgs QUIET)

# Desktop UI runner dependency (requires SDL2 development package for headers + pkg-config file)
pkg_check_modules(SDL2 QUIET IMPORTED_TARGET sdl2)

file(GLOB_RECURSE LVGL_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/lvgl/src/*.c
)

# ============================================================================
# CREATE ROS2 SUBSCRIBER LIBRARY
# ============================================================================

add_library(ros2subscriber STATIC
    ros2subscriber.cpp
)

target_compile_features(ros2subscriber PUBLIC cxx_std_17)

target_include_directories(ros2subscriber PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)

target_link_libraries(ros2subscriber Threads::Threads)
if(ament_cmake_FOUND)
    ament_target_dependencies(ros2subscriber rclcpp std_msgs)
else()
    target_link_libraries(ros2subscriber rclcpp::rclcpp)
endif()
if(lart_msgs_FOUND)
    target_compile_definitions(ros2subscriber PRIVATE LART_HAVE_DASHBOARD_STATE_MSG=1)
    if(ament_cmake_FOUND)
        ament_target_dependencies(ros2subscriber lart_msgs)
    endif()
    message(STATUS "ROS2 subscriber: lart_msgs detected (DashboardState supported)")
else()
    message(STATUS "ROS2 subscriber: lart_msgs not found (Float32 speed topic only)")
endif()

# Installation targets
install(TARGETS ros2subscriber
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)
install(FILES ros2subscriber.h DESTINATION include/${PROJECT_NAME})

if(SDL2_FOUND AND LVGL_SOURCES)
    add_library(lvgl STATIC ${LVGL_SOURCES})
    target_compile_features(lvgl PUBLIC c_std_99)
    target_include_directories(lvgl PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/lvgl
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/lvgl/src
        ${CMAKE_CURRENT_SOURCE_DIR}
    )
    target_compile_definitions(lvgl PUBLIC LV_CONF_INCLUDE_SIMPLE=1)

    set(UI_SOURCES
        eez-flow.cpp
        images.c
        screens.c
        speed_api.cpp
        styles.c
        ui.c
        ui_font_orbiter_bold_100.c
        ui_font_orbitron_15.c
        ui_font_orbitron_bold_15.c
        ui_font_orbitron_bold_20.c
        ui_font_orbitron_bold_25.c
        ui_font_orbitron_bold_30.c
        ui_font_orbitron_bold_40.c
        ui_font_orbitron_bold_50.c
        ui_image_lart_logo.c
        ui_runner.cpp
    )

    add_executable(ui_runner ${UI_SOURCES})

    target_compile_features(ui_runner PUBLIC cxx_std_17)
    target_include_directories(ui_runner PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/lvgl
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/lvgl/src
    )
    target_compile_definitions(ui_runner PRIVATE LV_CONF_INCLUDE_SIMPLE=1)
    target_link_libraries(ui_runner
        lvgl
        ros2subscriber
        PkgConfig::SDL2
        Threads::Threads
    )
    if(ament_cmake_FOUND)
        ament_target_dependencies(ui_runner rclcpp std_msgs)
    else()
        target_link_libraries(ui_runner rclcpp::rclcpp)
    endif()
    if(lart_msgs_FOUND)
        if(ament_cmake_FOUND)
            ament_target_dependencies(ui_runner lart_msgs)
        endif()
    endif()
else()
    message(STATUS "Desktop UI runner disabled: SDL2 (libsdl2-dev) or LVGL sources not available")
endif()

if(ament_cmake_FOUND)
    ament_package()
endif()