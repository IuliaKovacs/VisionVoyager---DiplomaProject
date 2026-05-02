# RouteRecordPlayer/CMakeLists.txt


set(SOURCES
    routeRecordPlayUtils.cpp
    routeRecordPlayUtils.h
)


add_library(RouteRecordPlayer STATIC ${SOURCES})


find_package(OpenCV REQUIRED)


if(BUILD_FOR_SIM)
    # Simulation dependencies (ROS 2 Jazzy)
    find_package(rclcpp REQUIRED)
    find_package(geometry_msgs REQUIRED)
    find_package(sensor_msgs REQUIRED)
    find_package(std_msgs REQUIRED)
    
    message(STATUS "RouteRecordPlayer: Configuring for SIMULATION (ROS 2)")
else()
    # Real hardware dependencies (Raspberry Pi)
    find_package(pybind11 REQUIRED)
    find_package(Python REQUIRED COMPONENTS Interpreter Development)
    
    message(STATUS "RouteRecordPlayer: Configuring for REAL HARDWARE")
endif()


if(BUILD_FOR_SIM)
    target_link_libraries(RouteRecordPlayer PRIVATE 
        VisionVoyagerMoves   # Links our HAL and ROS 2 indirectly
        RFID_Manager 
        ${OpenCV_LIBS}
        rclcpp::rclcpp
        # Explicitly link ROS 2 message typesupport for simulation
        geometry_msgs::geometry_msgs__rosidl_typesupport_cpp
        sensor_msgs::sensor_msgs__rosidl_typesupport_cpp
        std_msgs::std_msgs__rosidl_typesupport_cpp
    )
else()
    target_link_libraries(RouteRecordPlayer PRIVATE 
        VisionVoyagerMoves
        RFID_Manager 
        ${OpenCV_LIBS}
        pybind11::embed
    )
endif()


target_include_directories(RouteRecordPlayer PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${OpenCV_INCLUDE_DIRS}
    "../VisionVoyagerMoves"
    "../RFID"
    "../HardwareAbstractionLayer"
)

if(BUILD_FOR_SIM)
    target_include_directories(RouteRecordPlayer PRIVATE 
        ${rclcpp_INCLUDE_DIRS}
        ${geometry_msgs_INCLUDE_DIRS}
        ${sensor_msgs_INCLUDE_DIRS}
        ${std_msgs_INCLUDE_DIRS}
    )
else()
    target_include_directories(RouteRecordPlayer PRIVATE 
        ${pybind11_INCLUDE_DIRS}
    )
endif()