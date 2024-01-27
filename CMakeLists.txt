
cmake_minimum_required(VERSION 3.4)
project(VisionVoyager)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(pybind11 REQUIRED)

add_subdirectory(VisionVoyagerMoves)
add_subdirectory(RouteRegistration)


add_executable(VisionVoyager mainApplication.cpp)
target_link_libraries(VisionVoyager PRIVATE pybind11::embed curses VisionVoyagerMoves RouteRegistration)

# target_compile_definitions(VisionVoyager PRIVATE DEBUG)
