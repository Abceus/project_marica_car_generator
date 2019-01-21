cmake_minimum_required( VERSION 3.9 )

set( CMAKE_CXX_STANDARD 14 )

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Widgets CONFIG REQUIRED)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/ui)

qt5_wrap_cpp(MOC_FOR_UI
        includes/mainwindow.h
        includes/mainopenglwidget.h)

add_executable( ProjectMaricaCarGenerator
        ui/mainwindow.ui
        sources/main.cpp
        sources/mainwindow.cpp
        sources/mainopenglwidget.cpp
        sources/model.cpp
        sources/scene.cpp
        sources/unanimation.cpp
        sources/object.cpp
        ${MOC_FOR_UI})

target_include_directories( ProjectMaricaCarGenerator
        PRIVATE includes )

target_link_libraries(ProjectMaricaCarGenerator Qt5::Widgets)

set( INSTALL_PATH ${PROJECT_BINARY_DIR}/install )
install( DIRECTORY resources DESTINATION ${INSTALL_PATH} )
install( TARGETS ProjectMaricaCarGenerator
        DESTINATION ${INSTALL_PATH} )