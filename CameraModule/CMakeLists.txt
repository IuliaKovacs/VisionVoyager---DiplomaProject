set(SOURCES
    cameraModule.cpp
    cameraModule.h
)

add_library(CameraModule STATIC ${SOURCES})


find_package(OpenCV REQUIRED)


target_include_directories(CameraModule PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(CameraModule PRIVATE ${OpenCV_LIBS})