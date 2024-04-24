
cmake_minimum_required(VERSION 3.4)

project(VisionVoyager)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core Gui Multimedia MultimediaWidgets Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core Gui Multimedia MultimediaWidgets Quick)
find_package(pybind11 REQUIRED)
find_package(OpenXLSX REQUIRED)


add_subdirectory(VisionVoyagerMoves)
add_subdirectory(RouteRegistration)
add_subdirectory(RouteRecordPlayer)
add_subdirectory(KeyboardControl)
add_subdirectory(LineFollower)
add_subdirectory(ObstacleAvoidance)
add_subdirectory(CameraModule)
add_subdirectory(VoiceRecognition)
add_subdirectory(TextToSpeach)
add_subdirectory(ApplicationModule)
add_subdirectory(VisionVoyagerGUI)


# Specifying the name and path of the PocketSphinx library
set(Pocketsphinx_LIBRARY_DIRS "/home/kovaiu1/SRE/pocketsphinx/build")
set(Pocketsphinx_LIBRARIES "${Pocketsphinx_LIBRARY_DIRS}/libpocketsphinx.a")
set(POCKETSPHINX_INCLUDE_DIRS "/home/kovaiu1/SRE/pocketsphinx/include")
set(OpenXLSX_DIR "~/OpenXLSX/build")

add_executable(VisionVoyager mainApplication.cpp)

target_include_directories(VisionVoyagerGUI PRIVATE /usr/include/arm-linux-gnueabihf/qt5)

target_link_libraries(VisionVoyager PRIVATE pybind11::embed
                                            Qt${QT_VERSION_MAJOR}::Widgets 
                                            Qt${QT_VERSION_MAJOR}::Core 
                                            Qt${QT_VERSION_MAJOR}::Gui 
                                            Qt${QT_VERSION_MAJOR}::Multimedia
                                            Qt${QT_VERSION_MAJOR}::MultimediaWidgets
                                            Qt${QT_VERSION_MAJOR}::Quick            
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
                                            TextToSpeach
                                            Application
                                            VisionVoyagerGUI
                                            )




# target_compile_definitions(VisionVoyager PRIVATE DEBUG)
