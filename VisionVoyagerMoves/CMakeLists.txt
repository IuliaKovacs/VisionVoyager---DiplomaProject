set(SOURCES
    visionVoyager.cpp
    visionVoyager.h
)

add_library(VisionVoyagerMoves STATIC ${SOURCES})

target_include_directories(VisionVoyagerMoves PUBLIC 
    ${CMAKE_SOURCE_DIR}/HardwareAbstractionLayer
)

if(BUILD_FOR_SIM)
    # Use the variables found by find_package instead of ament_target_dependencies
    target_link_libraries(VisionVoyagerMoves PRIVATE 
        rclcpp::rclcpp
        geometry_msgs::geometry_msgs__rosidl_typesupport_cpp
        sensor_msgs::sensor_msgs__rosidl_typesupport_cpp
        std_msgs::std_msgs__rosidl_typesupport_cpp
    )
else()
    # --- REAL HARDWARE MODE ---
    # Find hardware-specific packages only when not in simulation
    find_package(pybind11 REQUIRED)
    find_package(Python REQUIRED COMPONENTS Interpreter Development)
    find_library(WIRINGPI_LIB wiringPi REQUIRED)

    target_include_directories(VisionVoyagerMoves PRIVATE ${pybind11_INCLUDE_DIRS})
    
    target_link_libraries(VisionVoyagerMoves PRIVATE 
        pybind11::module 
        ${WIRINGPI_LIB}
    )
endif()