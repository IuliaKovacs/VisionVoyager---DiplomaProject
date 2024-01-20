
cmake_minimum_required(VERSION 3.4)
project(VisionVoyager)

find_package(pybind11 REQUIRED)

add_subdirectory(VisionVoyagerMoves)

add_executable(VisionVoyager mainApplication.cpp)
target_link_libraries(VisionVoyager PRIVATE pybind11::embed curses VisionVoyagerMoves)
