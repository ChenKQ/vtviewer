cmake_minimum_required(VERSION 3.15)
project(vtviewer LANGUAGES CXX VERSION 0.1)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

#find_package(OpenGL REQUIRED)
#find_package(OpenCV REQUIRED)
#message("opengl functions !!!!!:" ${OpenGL_LIBRARIES})
#if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
#    list(APPEND LINK_LIBS "pthread")
#endif()

add_subdirectory(vtcore)

add_executable(vtviewer main.cpp
    src/mainwindow.cpp src/mainwindow.h src/mainwindow.ui
    resources/qss.qrc resources/icons.qrc
)
target_link_libraries(vtviewer Qt5::Gui vtcore
)
target_include_directories(vtviewer PUBLIC ${CMAKE_CURRENT_LIST_DIR})
set_property(TARGET vtviewer PROPERTY CXX_STANDARD 17)
