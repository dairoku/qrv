cmake_minimum_required(VERSION 3.18)
project(qrv)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(../libibc libibc_build)

include_directories(../libibc/include)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(qrv
        OpenDialog.ui
        qrv.ui
        qrv.qrc
        qrv.cpp
        OpenDialog.h
        qrv.h
        ../libibc/include/ibc/qt/image_view.h
        ../libibc/include/ibc/qt/image_scroll_area.h
        main.cpp)

target_link_libraries(qrv PRIVATE Qt5::Widgets)
