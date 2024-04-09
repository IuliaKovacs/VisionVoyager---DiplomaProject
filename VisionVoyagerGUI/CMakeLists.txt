
set(SOURCES
        adminModeWindow.cpp
        adminModeWindow.h
        cameraWidget.h
        cameraWidget.cpp
        fileDropWidget.h
        fileDropWidget.cpp
        adminmodewindow.ui
)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_library(VisionVoyagerGUI STATIC ${SOURCES})


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core Gui Multimedia MultimediaWidgets Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core Gui Multimedia MultimediaWidgets Quick)
find_package(OpenCV REQUIRED)



target_link_libraries(VisionVoyagerGUI PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets 
    Qt${QT_VERSION_MAJOR}::Core 
    Qt${QT_VERSION_MAJOR}::Gui 
    Qt${QT_VERSION_MAJOR}::Multimedia
    Qt${QT_VERSION_MAJOR}::MultimediaWidgets
    Qt${QT_VERSION_MAJOR}::Quick
    ${OpenCV_LIBS}
)


target_include_directories(VisionVoyagerGUI PRIVATE
    /usr/include/arm-linux-gnueabihf/qt5  
)




