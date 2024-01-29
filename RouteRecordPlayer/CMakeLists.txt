set(SOURCES
    routeRecordPlayUtils.cpp
    routeRecordPlayUtils.h
)

add_library(RouteRecordPlayer STATIC ${SOURCES})

find_package(pybind11 REQUIRED)
find_package(Python REQUIRED COMPONENTS Interpreter Development)


target_include_directories(RouteRecordPlayer PRIVATE ${pybind11_INCLUDE_DIRS})

