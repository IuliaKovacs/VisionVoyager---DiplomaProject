set(SOURCES
    application.cpp
    application.h
    application_utils.h
)

add_library(Application STATIC ${SOURCES})


set(OpenXLSX_DIR "~/OpenXLSX/build")


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core Gui Multimedia MultimediaWidgets Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core Gui Multimedia MultimediaWidgets Quick)
find_package(OpenCV REQUIRED)
find_package(OpenXLSX REQUIRED)


target_link_libraries(Application PRIVATE
    RFID_Manager
    Qt${QT_VERSION_MAJOR}::Widgets 
    Qt${QT_VERSION_MAJOR}::Core 
    Qt${QT_VERSION_MAJOR}::Gui 
    Qt${QT_VERSION_MAJOR}::Multimedia
    Qt${QT_VERSION_MAJOR}::MultimediaWidgets
    Qt${QT_VERSION_MAJOR}::Quick
    ${OpenCV_LIBS}
    VoiceRecognition
    OpenXLSX::OpenXLSX
)

target_include_directories(Application PRIVATE ${OpenCV_INCLUDE_DIRS})
target_include_directories(Application PRIVATE ${pybind11_INCLUDE_DIRS} ${OpenCV_LIBS})

include_directories(../RFID)
