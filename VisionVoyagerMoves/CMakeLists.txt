set(SOURCES
    visionVoyager.cpp
    visionVoyager.h
)

add_library(VisionVoyagerMoves STATIC ${SOURCES})

find_package(pybind11 REQUIRED)
find_package(Python REQUIRED COMPONENTS Interpreter Development)


target_include_directories(VisionVoyagerMoves PRIVATE ${pybind11_INCLUDE_DIRS})

#target_link_libraries(VisionVoyagerMoves PRIVATE pybind11::module)



