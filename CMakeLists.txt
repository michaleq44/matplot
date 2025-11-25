cmake_minimum_required(VERSION 4.0)
project(matplot)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

set(QT_FORCE_CMP0156_TO_VALUE NEW)

include_directories(inc)
qt_add_executable(matplot src/main.cpp src/window.cpp
    inc/main.hpp inc/window.hpp
        inc/math.hpp
        src/math.cpp)

target_link_libraries(matplot PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

