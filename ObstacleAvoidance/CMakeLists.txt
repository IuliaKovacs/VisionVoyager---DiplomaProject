# ObstacleAvoidance/CMakeLists.txt

set(SOURCES
    obstacleAvoidanceUtils.cpp
    obstacleAvoidanceUtils.h
)


add_library(ObstacleAvoidance STATIC ${SOURCES})


if(BUILD_FOR_SIM)
    find_package(rclcpp REQUIRED)
    find_package(geometry_msgs REQUIRED)
    find_package(sensor_msgs REQUIRED)
endif()


if(NOT BUILD_FOR_SIM)
    find_package(pybind11 REQUIRED)
    find_package(Python REQUIRED COMPONENTS Interpreter Development)
endif()


if(BUILD_FOR_SIM)
    target_link_libraries(ObstacleAvoidance PRIVATE 
        VisionVoyagerMoves   # Links our HAL and ROS 2
        rclcpp::rclcpp
        geometry_msgs::geometry_msgs__rosidl_typesupport_cpp
        sensor_msgs::sensor_msgs__rosidl_generator_cpp
    )
else()
    target_link_libraries(ObstacleAvoidance PRIVATE 
        VisionVoyagerMoves
        pybind11::embed
    )
endif()


target_include_directories(ObstacleAvoidance PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    "../VisionVoyagerMoves"
    "../HardwareAbstractionLayer"
)


if(BUILD_FOR_SIM)
    target_include_directories(ObstacleAvoidance PRIVATE ${rclcpp_INCLUDE_DIRS})
else()
    target_include_directories(ObstacleAvoidance PRIVATE ${pybind11_INCLUDE_DIRS})
endif()