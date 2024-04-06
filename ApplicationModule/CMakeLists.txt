set(SOURCES
    application.cpp
    application.h
)

add_library(Application STATIC ${SOURCES})

target_include_directories(Application PRIVATE ${pybind11_INCLUDE_DIRS})

