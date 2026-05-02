cmake_minimum_required(VERSION 3.4)

project(VisionVoyager)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- HAL TOGGLE START ---
# Define the build option (Default is OFF for real hardware)
option(BUILD_FOR_SIM "Build for Gazebo simulation" OFF)

if(BUILD_FOR_SIM)
    add_definitions(-DUSE_SIMULATION)
    message(STATUS "Build mode: SIMULATION (Gazebo)")
    # ROS 2 packages are needed globally for the HAL bridge
    find_package(rclcpp REQUIRED)
    find_package(geometry_msgs REQUIRED)
    find_package(sensor_msgs REQUIRED)
    find_package(std_msgs REQUIRED)
else()
    message(STATUS "Build mode: REAL HARDWARE (Picar-X)")
    find_package(pybind11 REQUIRED)
endif()
# --- HAL TOGGLE END ---

find_package(OpenCV REQUIRED)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core Gui Multimedia MultimediaWidgets Quick Charts)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core Gui Multimedia MultimediaWidgets Quick Charts)
find_package(OpenXLSX REQUIRED)


add_subdirectory(HardwareAbstractionLayer) 
add_subdirectory(VisionVoyagerMoves)
add_subdirectory(RouteRegistration)
add_subdirectory(RouteRecordPlayer)
add_subdirectory(KeyboardControl)
add_subdirectory(LineFollower)
add_subdirectory(ObstacleAvoidance)
add_subdirectory(CameraModule)
add_subdirectory(VoiceRecognition)
add_subdirectory(TextToSpeech)
add_subdirectory(ApplicationModule)
add_subdirectory(VisionVoyagerGUI)
add_subdirectory(RFID)

# PocketSphinx configuration
set(Pocketsphinx_LIBRARY_DIRS "/home/kovaiu1/SRE/pocketsphinx/build")
#set(Pocketsphinx_LIBRARIES "${Pocketsphinx_LIBRARY_DIRS}/libpocketsphinx.a")
set(POCKETSPHINX_INCLUDE_DIRS "/home/kovaiu1/SRE/pocketsphinx/include")
set(OpenXLSX_DIR "~/OpenXLSX/build")

add_executable(VisionVoyager mainApplication.cpp)

target_include_directories(VisionVoyagerGUI PRIVATE /usr/include/arm-linux-gnueabihf/qt5 ${Qt5Charts_INCLUDE_DIRS})

# Include HAL directory globally for all modules
target_include_directories(VisionVoyager PRIVATE ${CMAKE_SOURCE_DIR}/HardwareAbstractionLayer)

target_link_libraries(VisionVoyager PRIVATE 
                                            RFID_Manager
                                            Qt${QT_VERSION_MAJOR}::Widgets 
                                            Qt${QT_VERSION_MAJOR}::Core 
                                            Qt${QT_VERSION_MAJOR}::Gui 
                                            Qt${QT_VERSION_MAJOR}::Multimedia
                                            Qt${QT_VERSION_MAJOR}::MultimediaWidgets
                                            Qt${QT_VERSION_MAJOR}::Quick
                                            Qt${QT_VERSION_MAJOR}::Charts            
                                            ${OpenCV_LIBS}
                                            curses 
                                            ${Pocketsphinx_LIBRARIES}
                                            pthread
                                            OpenXLSX::OpenXLSX
                                            VisionVoyagerMoves 
                                            RouteRegistration 
                                            RouteRecordPlayer 
                                            KeyboardControl
                                            LineFollower
                                            ObstacleAvoidance
                                            CameraModule
                                            VoiceRecognition
                                            TextToSpeech
                                            Application
                                            VisionVoyagerGUI
                                            )

# Conditionally link hardware-specific or simulation-specific libraries
if(BUILD_FOR_SIM)
    target_link_libraries(VisionVoyager PRIVATE 
        rclcpp::rclcpp 
        geometry_msgs::geometry_msgs__rosidl_typesupport_cpp
        sensor_msgs::sensor_msgs__rosidl_typesupport_cpp
        std_msgs::std_msgs__rosidl_typesupport_cpp
    )
else()
    target_link_libraries(VisionVoyager PRIVATE pybind11::embed)
endif()

# target_compile_definitions(VisionVoyager PRIVATE DEBUG)