set(SOURCES
    voiceRecognition.cpp
    voiceRecognition.h
)

add_library(VoiceRecognition STATIC ${SOURCES})

# PocketSphinx is installed in a custom location and I need to provide the paths
set(POCKETSPHINX_LIBRARIES "/home/kovaiu1/SRE/pocketsphinx/build/libpocketsphinx.a")
set(POCKETSPHINX_INCLUDE_DIRS "/home/kovaiu1/SRE/pocketsphinx/include")

target_include_directories(VoiceRecognition PRIVATE ${POCKETSPHINX_INCLUDE_DIRS})
target_link_libraries(VoiceRecognition PRIVATE ${POCKETSPHINX_LIBRARIES})


