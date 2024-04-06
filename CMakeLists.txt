
cmake_minimum_required(VERSION 3.4)
project(VisionVoyager)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(pybind11 REQUIRED)


add_subdirectory(VisionVoyagerMoves)
add_subdirectory(RouteRegistration)
add_subdirectory(RouteRecordPlayer)
add_subdirectory(KeyboardControl)
add_subdirectory(LineFollower)
add_subdirectory(ObstacleAvoidance)
add_subdirectory(VoiceRecognition)
add_subdirectory(TextToSpeach)
add_subdirectory(ApplicationModule)


# Specifying the name and path of the PocketSphinx library
set(Pocketsphinx_LIBRARY_DIRS "/home/kovaiu1/SRE/pocketsphinx/build")
set(Pocketsphinx_LIBRARIES "${Pocketsphinx_LIBRARY_DIRS}/libpocketsphinx.a")
set(POCKETSPHINX_INCLUDE_DIRS "/home/kovaiu1/SRE/pocketsphinx/include")


add_executable(VisionVoyager mainApplication.cpp)


target_link_libraries(VisionVoyager PRIVATE pybind11::embed 
                                            curses 
                                            ${Pocketsphinx_LIBRARIES}
                                            VisionVoyagerMoves 
                                            RouteRegistration 
                                            RouteRecordPlayer 
                                            KeyboardControl
                                            LineFollower
                                            ObstacleAvoidance
                                            VoiceRecognition
                                            TextToSpeach
                                            Application
                                            )

# target_compile_definitions(VisionVoyager PRIVATE DEBUG)
