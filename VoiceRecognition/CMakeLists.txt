set(SOURCES
    voiceRecognition.cpp
    voiceRecognition.h
)

add_library(VoiceRecognition STATIC ${SOURCES})

set(Pocketsphinx_LIBRARY_DIRS "/home/kovaiu1/SRE/pocketsphinx/build")

# Specify the name of the PocketSphinx library
#set(Pocketsphinx_LIBRARIES "${Pocketsphinx_LIBRARY_DIRS}/libpocketsphinx.a")
set(POCKETSPHINX_INCLUDE_DIRS "/home/kovaiu1/SRE/pocketsphinx/include")

target_include_directories(VoiceRecognition PRIVATE ${POCKETSPHINX_INCLUDE_DIRS})
target_link_libraries(VoiceRecognition PRIVATE ${Pocketsphinx_LIBRARIES})


