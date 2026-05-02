# KeyboardControl/CMakeLists.txt

set(SOURCES
    keyboardControl.cpp
    keyboardControl.h
)


add_library(KeyboardControl STATIC ${SOURCES})


find_package(Curses REQUIRED)
find_package(OpenCV REQUIRED)


if(BUILD_FOR_SIM)
    find_package(rclcpp REQUIRED)
    find_package(geometry_msgs REQUIRED)
else()
    # Real hardware dependencies (Raspberry Pi)
    find_package(pybind11 REQUIRED)
    find_package(Python REQUIRED COMPONENTS Interpreter Development)
endif()


if(BUILD_FOR_SIM)
    target_link_libraries(KeyboardControl PRIVATE 
        VisionVoyagerMoves   # Links HAL and ROS 2
        ${CURSES_LIBRARIES}
        ${OpenCV_LIBS}
        rclcpp::rclcpp
        geometry_msgs::geometry_msgs__rosidl_typesupport_cpp
        sensor_msgs::sensor_msgs__rosidl_typesupport_cpp
    )
else()
    target_link_libraries(KeyboardControl PRIVATE 
        VisionVoyagerMoves
        ${CURSES_LIBRARIES}
        ${OpenCV_LIBS}
        pybind11::embed
    )
endif()


target_include_directories(KeyboardControl PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CURSES_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
    "../VisionVoyagerMoves"
    "../HardwareAbstractionLayer"
)

# Special includes for ROS 2 or Python depending on the mode
if(BUILD_FOR_SIM)
    target_include_directories(KeyboardControl PRIVATE ${rclcpp_INCLUDE_DIRS})
else()
    target_include_directories(KeyboardControl PRIVATE ${pybind11_INCLUDE_DIRS})
endif()