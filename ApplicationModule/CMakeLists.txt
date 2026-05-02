# ApplicationModule/CMakeLists.txt

set(SOURCES
    application.cpp
    application.h
    application_utils.h
)


add_library(Application STATIC ${SOURCES})


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core Gui Multimedia MultimediaWidgets Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core Gui Multimedia MultimediaWidgets Quick)
find_package(OpenCV REQUIRED)
find_package(OpenXLSX REQUIRED)


if(BUILD_FOR_SIM)
    find_package(rclcpp REQUIRED)
    find_package(geometry_msgs REQUIRED)
    find_package(sensor_msgs REQUIRED)
endif()


target_link_libraries(Application PRIVATE
    RFID_Manager
    Qt${QT_VERSION_MAJOR}::Widgets 
    Qt${QT_VERSION_MAJOR}::Core 
    Qt${QT_VERSION_MAJOR}::Gui 
    Qt${QT_VERSION_MAJOR}::Multimedia
    Qt${QT_VERSION_MAJOR}::MultimediaWidgets
    Qt${QT_VERSION_MAJOR}::Quick
    ${OpenCV_LIBS}
    OpenXLSX::OpenXLSX
    # Logic modules
    VisionVoyagerMoves
    RouteRecordPlayer
    KeyboardControl
    LineFollower
    ObstacleAvoidance
)

if(BUILD_FOR_SIM)
    target_link_libraries(Application PRIVATE 
        rclcpp::rclcpp
        geometry_msgs::geometry_msgs__rosidl_typesupport_cpp
        sensor_msgs::sensor_msgs__rosidl_generator_cpp
    )
else()
    # Link VoiceRecognition and Python only for Real Hardware
    target_link_libraries(Application PRIVATE VoiceRecognition)
    find_package(pybind11 REQUIRED)
    target_link_libraries(Application PRIVATE pybind11::embed)
endif()


target_include_directories(Application PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${OpenCV_INCLUDE_DIRS}
    "../RFID"
    "../VisionVoyagerMoves"
    "../VisionVoyagerGUI"
    "../KeyboardControl"
    "../RouteRecordPlayer"
    "../VoiceRecognition"
    "../HardwareAbstractionLayer"
)

if(BUILD_FOR_SIM)
    target_include_directories(Application PRIVATE ${rclcpp_INCLUDE_DIRS})
else()
    target_include_directories(Application PRIVATE ${pybind11_INCLUDE_DIRS})
endif()