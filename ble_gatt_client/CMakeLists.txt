cmake_minimum_required(VERSION 3.5)
project(ble_gatt_client)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(ament_cmake_auto REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rcl_interfaces REQUIRED)
find_package(telemetry_interfaces REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

include_directories(include)

ament_auto_add_library(BleGattClient SHARED
  src/ble_gatt_client.cpp
)

target_compile_features(BleGattClient PRIVATE cxx_std_17)
ament_target_dependencies(BleGattClient rclcpp rcl_interfaces
  std_msgs telemetry_interfaces)
target_include_directories(BleGattClient
  PUBLIC
  #$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  #$<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

add_executable(BleGattClient_node src/ble_gatt_client_node.cpp)
ament_target_dependencies(BleGattClient_node rclcpp telemetry_interfaces)
target_link_libraries(BleGattClient_node BleGattClient)

install(TARGETS
BleGattClient
  DESTINATION lib/${PROJECT_NAME})

install(TARGETS
  BleGattClient_node
  DESTINATION lib/${PROJECT_NAME})


ament_package()
