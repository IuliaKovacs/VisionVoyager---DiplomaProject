set(SOURCES
    lineFollower.cpp
    lineFollower.h
)

add_library(LineFollower STATIC ${SOURCES})

find_package(pybind11 REQUIRED)
find_package(Python REQUIRED COMPONENTS Interpreter Development)


target_include_directories(LineFollower PRIVATE ${pybind11_INCLUDE_DIRS})

