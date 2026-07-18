cmake_minimum_required(VERSION 3.5)
project(drone_control)

# Default to C99
if(NOT CMAKE_C_STANDARD)
	set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
	set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Include directories for header files
include_directories(include)

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(builtin_interfaces REQUIRED)
find_package(px4_msgs REQUIRED)
find_package(rclcpp REQUIRED)


# offboard_control
add_executable(takeoff src/takeoff.cpp src/drone_control.cpp)
ament_target_dependencies(takeoff rclcpp px4_msgs)
install(TARGETS takeoff DESTINATION lib/${PROJECT_NAME})

# my_node
add_executable(arm src/arm.cpp src/drone_control.cpp)
ament_target_dependencies(arm rclcpp px4_msgs)
install(TARGETS arm DESTINATION lib/${PROJECT_NAME})

#flight
add_executable(flight src/flight.cpp src/drone_control.cpp) 
ament_target_dependencies(flight rclcpp px4_msgs)
install(TARGETS flight DESTINATION lib/${PROJECT_NAME})

#oflight
add_executable(oflight src/oflight.cpp src/drone_control.cpp) 
ament_target_dependencies(oflight rclcpp px4_msgs)
install(TARGETS oflight DESTINATION lib/${PROJECT_NAME})

#8flight
add_executable(8flight src/8flight.cpp src/drone_control.cpp) 
ament_target_dependencies(8flight rclcpp px4_msgs)
install(TARGETS 8flight DESTINATION lib/${PROJECT_NAME})

# Export information to downstream packages
ament_export_dependencies(rosidl_default_runtime)

ament_package()
