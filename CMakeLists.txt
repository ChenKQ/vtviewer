cmake_minimum_required(VERSION 3.10)
project(vtviewer LANGUAGES CXX VERSION 0.1)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

add_subdirectory(brick)

add_executable(vtviewer main.cpp
    src/mainwindow.cpp src/mainwindow.h src/mainwindow.ui)
target_link_libraries(vtviewer Qt5::Core Qt5::Gui Qt5::Widgets)
