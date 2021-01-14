project(vtcore LANGUAGES CXX)

set(CMAKE_AUTORCC ON)
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
find_package(OpenGL REQUIRED)
find_package(OpenCV REQUIRED)
if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    list(APPEND LINK_LIBS "pthread")
endif()

add_library(vtcore SHARED shaders/shaders.qrc
    include/vtcore/render/coordinate.h src/render/coordinate.cpp
    include/vtcore/render/view.h src/render/view.cpp
    include/vtcore/render/glvideorender.h src/render/glvideorender.cpp
    include/vtcore/render/glvideowidget.h src/render/glvideowidget.cpp

    include/vtcore/io/videocapture.h src/io/videocapture.cpp
    include/vtcore/io/playmanager.h src/io/playmanager.cpp
)
target_include_directories(vtcore PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(vtcore PUBLIC Qt5::Core Qt5::Gui Qt5::Widgets
                                    ${OPENGL_LIBRARY} ${OpenCV_LIBS} ${LINK_LIBS}
)
