set(SOURCES
    keyboardControl.cpp
    keyboardControl.h
)

add_library(KeyboardControl STATIC ${SOURCES})

find_package(pybind11 REQUIRED)
find_package(Python REQUIRED COMPONENTS Interpreter Development)


target_include_directories(KeyboardControl PRIVATE ${pybind11_INCLUDE_DIRS})

