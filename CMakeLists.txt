cmake_minimum_required(VERSION 3.15)
project(vtviewer LANGUAGES CXX VERSION 0.1)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

find_package(OpenGL REQUIRED)
message("opengl functions !!!!!:" ${OpenGL_LIBRARIES})
add_subdirectory(brick)

add_executable(vtviewer main.cpp
    src/mainwindow.cpp src/mainwindow.h src/mainwindow.ui
    src/glvideorender.h src/glvideorender.cpp
    src/glvideowidget.h src/glvideowidget.cpp
    resources/qss.qrc resources/icons.qrc resources/shaders.qrc
    src/utils.h src/utils.cpp
)
target_link_libraries(vtviewer Qt5::Core Qt5::Gui Qt5::Widgets
                                brick ${OPENGL_LIBRARY}
)
target_include_directories(vtviewer PUBLIC ${CMAKE_CURRENT_LIST_DIR})
