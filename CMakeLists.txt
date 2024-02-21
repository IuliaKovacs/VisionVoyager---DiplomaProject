
cmake_minimum_required(VERSION 3.4)
project(VisionVoyager)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(pybind11 REQUIRED)
find_package(OpenCV REQUIRED)

add_subdirectory(VisionVoyagerMoves)
add_subdirectory(RouteRegistration)
add_subdirectory(RouteRecordPlayer)
add_subdirectory(KeyboardControl)
add_subdirectory(LineFollower)
add_subdirectory(ObstacleAvoidance)
add_subdirectory(CameraModule)


add_executable(VisionVoyager mainApplication.cpp)
target_link_libraries(VisionVoyager PRIVATE pybind11::embed 
                                            ${OpenCV_LIBS}
                                            curses 
                                            VisionVoyagerMoves 
                                            RouteRegistration 
                                            RouteRecordPlayer 
                                            KeyboardControl
                                            LineFollower
                                            ObstacleAvoidance
                                            CameraModule)

# target_compile_definitions(VisionVoyager PRIVATE DEBUG)
