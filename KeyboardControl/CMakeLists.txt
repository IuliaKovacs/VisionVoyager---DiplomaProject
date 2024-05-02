set(SOURCES
    keyboardControl.cpp
    keyboardControl.h
)

add_library(KeyboardControl STATIC ${SOURCES})

find_package(pybind11 REQUIRED)
find_package(Python REQUIRED COMPONENTS Interpreter Development)
find_package(Curses REQUIRED)
find_package(OpenCV REQUIRED)

target_link_libraries(KeyboardControl PRIVATE ${CURSES_LIBRARIES})
target_include_directories(KeyboardControl PRIVATE ${pybind11_INCLUDE_DIRS} ${CURSES_INCLUDE_DIRS})

