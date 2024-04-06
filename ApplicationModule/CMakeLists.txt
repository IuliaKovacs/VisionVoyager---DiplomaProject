set(SOURCES
    application.cpp
    application.h
)

add_library(Application STATIC ${SOURCES})

find_package(OpenCV REQUIRED)


target_include_directories(Application PRIVATE ${OpenCV_INCLUDE_DIRS})
target_include_directories(Application PRIVATE ${pybind11_INCLUDE_DIRS} ${OpenCV_LIBS})

