cmake_minimum_required( VERSION 3.9 )

project( ProjectMaricaCarGenerator )

set( CMAKE_CXX_STANDARD 20 )

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Widgets CONFIG REQUIRED)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/ui)

qt5_wrap_cpp(MOC_FOR_UI
        includes/mainwindow.h
        includes/mainopenglwidget.h
        includes/openglsimulationwidget.h)

if(DEFINED BULLET_LIBRARY_DIRECTORY)
    link_directories(${BULLET_LIBRARY_DIRECTORY})
endif()

add_executable( ProjectMaricaCarGenerator
        ui/mainwindow.ui
        sources/main.cpp
        sources/mainwindow.cpp
        sources/mainopenglwidget.cpp
        sources/resources/model.cpp
        sources/render_system/scene.cpp
        sources/resources/unanimation.cpp
        sources/object.cpp
        sources/resources/resource_manager.cpp
        sources/render_system/mesh.cpp
        sources/openglsimulationwidget.cpp
        sources/physics/physobject.cpp
        sources/physics/physicworld.cpp
        sources/render_system/drawable.cpp
        sources/render_system/scene_node.cpp
        sources/render_system/wireframe.cpp
        sources/render_system/renderer.cpp
        sources/render_system/garbage_collector.cpp
        sources/utils/scope_guard.cpp
        ${MOC_FOR_UI})
        
if(NOT DEFINED BULLET_INCLUDE_DIRECTORY)
    set(BULLET_INCLUDE_DIRECTORY /usr/include/bullet)
endif()

target_include_directories( ProjectMaricaCarGenerator
        PRIVATE includes ${BULLET_INCLUDE_DIRECTORY})

target_link_libraries(ProjectMaricaCarGenerator Qt5::Widgets BulletDynamics BulletCollision LinearMath)

target_compile_options(ProjectMaricaCarGenerator PRIVATE -Werror=unused-result)

# if (UNIX)
# target_link_libraries(ProjectMaricaCarGenerator Qt5::Widgets
#         ${CMAKE_CURRENT_SOURCE_DIR}/third_party/bullet/libs/BulletDynamics/libBulletDynamics.a
#         ${CMAKE_CURRENT_SOURCE_DIR}/third_party/bullet/libs/BulletCollision/libBulletCollision.a
#         ${CMAKE_CURRENT_SOURCE_DIR}/third_party/bullet/libs/LinearMath/libLinearMath.a)
# endif (UNIX)

# if (MSVC)
# target_link_libraries(ProjectMaricaCarGenerator Qt5::Widgets
#         ${CMAKE_CURRENT_SOURCE_DIR}/third_party/bullet/libs/Windows/Debug/BulletDynamics_Debug.lib
#         ${CMAKE_CURRENT_SOURCE_DIR}/third_party/bullet/libs/Windows/Debug/BulletCollision_Debug.lib
#         ${CMAKE_CURRENT_SOURCE_DIR}/third_party/bullet/libs/Windows/Debug/LinearMath_Debug.lib)
# endif (MSVC)

set( INSTALL_PATH ${PROJECT_BINARY_DIR}/install )
install( DIRECTORY resources DESTINATION ${INSTALL_PATH} )
install( TARGETS ProjectMaricaCarGenerator
        DESTINATION ${INSTALL_PATH} )
