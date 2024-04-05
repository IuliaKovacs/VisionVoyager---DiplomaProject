set(SOURCES
    textToSpeach.cpp
    textToSpeach.h
)

add_library(TextToSpeach STATIC ${SOURCES})

target_include_directories(TextToSpeach PRIVATE ${pybind11_INCLUDE_DIRS})

