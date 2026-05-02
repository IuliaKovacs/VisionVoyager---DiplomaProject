# LineFollower/CMakeLists.txt

set(SOURCES
    lineFollower.cpp
    lineFollower.h
)


add_library(LineFollower STATIC ${SOURCES})


if(BUILD_FOR_SIM)
    find_package(rclcpp REQUIRED)
    find_package(geometry_msgs REQUIRED)
endif()


if(NOT BUILD_FOR_SIM)
    find_package(pybind11 REQUIRED)
    find_package(Python REQUIRED COMPONENTS Interpreter Development)
endif()


if(BUILD_FOR_SIM)
    target_link_libraries(LineFollower PRIVATE 
        VisionVoyagerMoves   # Links our HAL and ROS 2 indirectly
        rclcpp::rclcpp
        geometry_msgs::geometry_msgs__rosidl_typesupport_cpp
    )
else()
    target_link_libraries(LineFollower PRIVATE 
        VisionVoyagerMoves
        pybind11::embed
    )
endif()


target_include_directories(LineFollower PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    "../VisionVoyagerMoves"
    "../HardwareAbstractionLayer"
)

if(BUILD_FOR_SIM)
    target_include_directories(LineFollower PRIVATE ${rclcpp_INCLUDE_DIRS})
else()
    target_include_directories(LineFollower PRIVATE ${pybind11_INCLUDE_DIRS})
endif()