cmake_minimum_required(VERSION 3.8)
project(speed_subscriber LANGUAGES C CXX)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake QUIET)
find_package(PkgConfig QUIET)


# Prefer pkg-config, but fall back to direct CMake lookup when pkg-config
# is not available in the build environment (common in EEZ Studio setups).
if(PkgConfig_FOUND)
	pkg_check_modules(MOSQUITTO QUIET libmosquitto)
endif()

if(NOT MOSQUITTO_FOUND)
	find_path(MOSQUITTO_INCLUDE_DIR mosquitto.h)
	find_library(MOSQUITTO_LIBRARY mosquitto)

	if(MOSQUITTO_INCLUDE_DIR AND MOSQUITTO_LIBRARY)
		set(MOSQUITTO_FOUND TRUE)
		set(MOSQUITTO_INCLUDE_DIRS ${MOSQUITTO_INCLUDE_DIR})
		set(MOSQUITTO_LIBRARIES ${MOSQUITTO_LIBRARY})
	endif()
endif()

add_library(mqttsubscriber STATIC
	mqttsubscriber.cpp
)

target_compile_features(mqttsubscriber PUBLIC cxx_std_17)
target_include_directories(mqttsubscriber PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
	$<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)

if(MOSQUITTO_FOUND)
	target_compile_definitions(mqttsubscriber PRIVATE LART_ENABLE_MQTT_LIBMOSQUITTO=1)
	target_include_directories(mqttsubscriber PRIVATE ${MOSQUITTO_INCLUDE_DIRS})
	target_link_libraries(mqttsubscriber PRIVATE ${MOSQUITTO_LIBRARIES})
	message(STATUS "MQTT dependencies found: building mqttsubscriber with live subscription support")
else()
	message(WARNING "MQTT dependencies not found: building mqttsubscriber in fallback mode (install libmosquitto-dev, and pkg-config if available)")
endif()

install(TARGETS mqttsubscriber
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
	RUNTIME DESTINATION bin
)
install(FILES mqttsubscriber.h DESTINATION include/${PROJECT_NAME})

if(ament_cmake_FOUND)
	ament_package()
endif()