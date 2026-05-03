# VisionVoyagerGUI/CMakeLists.txt

set(SOURCES
    adminModeWindow.h
    adminModeWindow.cpp
    adminModeUtils.cpp
    cameraWidget.h
    cameraWidget.cpp
    fileDropWidget.h
    fileDropWidget.cpp
    adminmodewindow.ui
    DataCollection/dataCollectionUtils.cpp
)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Avoid ARM-specific paths on x86 simulation environment
if(NOT BUILD_FOR_SIM)
    set(CMAKE_PREFIX_PATH "/usr/lib/arm-linux-gnueabihf/cmake;/usr/include/arm-linux-gnueabihf/qt5")
endif()

add_library(VisionVoyagerGUI STATIC ${SOURCES})


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core Gui Multimedia MultimediaWidgets Quick Charts)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core Gui Multimedia MultimediaWidgets Quick Charts)
find_package(OpenCV REQUIRED)
find_package(OpenXLSX REQUIRED)


if(BUILD_FOR_SIM)
    find_package(rclcpp REQUIRED)
    find_package(geometry_msgs REQUIRED) 
    find_package(sensor_msgs REQUIRED)
    find_package(cv_bridge REQUIRED)
endif()


target_link_libraries(VisionVoyagerGUI PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets 
    Qt${QT_VERSION_MAJOR}::Core 
    Qt${QT_VERSION_MAJOR}::Gui 
    Qt${QT_VERSION_MAJOR}::Multimedia
    Qt${QT_VERSION_MAJOR}::MultimediaWidgets
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::Charts
    ${OpenCV_LIBS}
    OpenXLSX::OpenXLSX
)

if(BUILD_FOR_SIM)
    target_link_libraries(VisionVoyagerGUI PRIVATE 
        rclcpp::rclcpp
        geometry_msgs::geometry_msgs__rosidl_typesupport_cpp
        sensor_msgs::sensor_msgs__rosidl_typesupport_cpp
        cv_bridge::cv_bridge
    )
endif()


target_include_directories(VisionVoyagerGUI PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${OpenCV_INCLUDE_DIRS}
    "../VisionVoyagerMoves"
    "../HardwareAbstractionLayer"
)

if(BUILD_FOR_SIM)
    target_include_directories(VisionVoyagerGUI PRIVATE 
        ${rclcpp_INCLUDE_DIRS}
        ${geometry_msgs_INCLUDE_DIRS}
        ${sensor_msgs_INCLUDE_DIRS}
        ${cv_bridge_INCLUDE_DIRS}
    )
else()
    target_include_directories(VisionVoyagerGUI PRIVATE /usr/include/arm-linux-gnueabihf/qt5)
endif()

# add_definitions(-DNCURSES_NOMACROS)
# add_definitions(-DQT_NO_KEYWORDS)