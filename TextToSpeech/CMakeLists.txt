set(SOURCES
    textToSpeech.cpp
    textToSpeech.h
)

add_library(TextToSpeech STATIC ${SOURCES})

target_include_directories(TextToSpeech PRIVATE ${pybind11_INCLUDE_DIRS})

