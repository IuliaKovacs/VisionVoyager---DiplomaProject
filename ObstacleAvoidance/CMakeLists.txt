set(SOURCES
    obstacleAvoidanceUtils.cpp
    obstacleAvoidanceUtils.h
)

add_library(ObstacleAvoidance STATIC ${SOURCES})

find_package(pybind11 REQUIRED)
find_package(Python REQUIRED COMPONENTS Interpreter Development)


target_include_directories(ObstacleAvoidance PRIVATE ${pybind11_INCLUDE_DIRS})

