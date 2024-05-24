set(SOURCES
    routeRecordPlayUtils.cpp
    routeRecordPlayUtils.h
)

add_library(RouteRecordPlayer STATIC ${SOURCES})

find_package(pybind11 REQUIRED)
find_package(Python REQUIRED COMPONENTS Interpreter Development)
find_package(OpenCV REQUIRED)

target_link_libraries(RouteRecordPlayer PRIVATE ${OpenCV_LIBS})

target_include_directories(RouteRecordPlayer PRIVATE ${pybind11_INCLUDE_DIRS} ${OpenCV_LIBS})

