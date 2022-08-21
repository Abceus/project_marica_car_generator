cmake_minimum_required( VERSION 3.9 )

project( ProjectMaricaCarGenerator )

set( CMAKE_CXX_STANDARD 20 )

set(BUILD_SHARED_LIBS OFF)
set(wxUSE_STL ON)
set(_CRT_SECURE_NO_WARNINGS ON)
add_subdirectory(3rd_party/wxWidgets)

set(SOURCES sources/main.cpp

        sources/widgets/app.cpp
        sources/widgets/mainwindow.cpp
        sources/widgets/openglview.cpp
        sources/widgets/configurationWidget.cpp

        sources/render_system/texture.cpp
        sources/render_system/batch.cpp
        sources/render_system/element_buffer.cpp
        sources/render_system/fragment_shader.cpp
        sources/render_system/triangle_array.cpp
        sources/render_system/edge_array.cpp
        sources/render_system/vertex_buffer.cpp
        sources/render_system/vertex_shader.cpp
        sources/render_system/shader_program.cpp
        sources/render_system/mesh.cpp
        sources/render_system/wireframe.cpp
        sources/render_system/camera.cpp
        sources/render_system/camera_manager.cpp
        sources/render_system/scene.cpp
        sources/render_system/scene_node.cpp
        sources/render_system/renderer.cpp
        sources/render_system/garbage_collector.cpp
        sources/render_system/color.cpp

        sources/render_system/camera/free_camera_controller.cpp

        sources/resources/model.cpp
        sources/resources/unanimation.cpp

        sources/physics/physicworld.cpp
        sources/physics/physobject.cpp

        sources/utils/math/angle.cpp
        sources/utils/math/rot3.cpp
        sources/utils/math/utils.cpp)

if(WIN32)
        add_executable( ProjectMaricaCarGenerator WIN32
                ${SOURCES})
else()
        add_executable( ProjectMaricaCarGenerator
                ${SOURCES})
endif()

find_package(OpenGL REQUIRED)

set(GLEW_USE_STATIC_LIBS TRUE)
add_subdirectory(3rd_party/glew)

add_subdirectory(3rd_party/glm)

set(USE_GRAPHICAL_BENCHMARK OFF CACHE BOOL "Use Graphical Benchmark" FORCE)
set(ENABLE_VHACD OFF CACHE BOOL "Use VHACD in BulletRobotics and pybullet" FORCE)
set(BUILD_CPU_DEMOS OFF CACHE BOOL "Build original Bullet CPU examples" FORCE)
set(USE_GLUT OFF CACHE BOOL "Use Glut" FORCE)
set(BUILD_ENET OFF CACHE BOOL "Set when you want to build apps with enet UDP networking support" FORCE)
set(BUILD_CLSOCKET OFF CACHE BOOL "Set when you want to build apps with enet TCP networking support" FORCE)
set(BUILD_OPENGL3_DEMOS OFF CACHE BOOL "Set when you want to build the OpenGL3+ demos" FORCE)
set(BUILD_BULLET2_DEMOS OFF CACHE BOOL "Set when you want to build the Bullet 2 demos" FORCE)
set(BUILD_EXTRAS OFF CACHE BOOL "Set when you want to build the extras" FORCE)
set(INSTALL_LIBS OFF CACHE BOOL "Set when you want to install libraries" FORCE)
set(INSTALL_CMAKE_FILES OFF CACHE BOOL "Install generated CMake files" FORCE)
set(BUILD_UNIT_TESTS OFF CACHE BOOL "Build Unit Tests" FORCE)
set(USE_MSVC_RUNTIME_LIBRARY_DLL ON CACHE BOOL "Use MSVC Runtime Library DLL (/MD or /MDd)" FORCE)
set(USE_MSVC_RELEASE_RUNTIME_ALWAYS OFF CACHE BOOL "Use MSVC Release Runtime Library even in Debug" FORCE)
set(BUILD_BULLET3 OFF CACHE BOOL "Set when you want to build Bullet 3" FORCE)
add_subdirectory(3rd_party/bullet-physics)

target_include_directories( ProjectMaricaCarGenerator
        PRIVATE includes ${BULLET_PHYSICS_SOURCE_DIR}/src)

target_link_libraries(ProjectMaricaCarGenerator wx::core wx::base wx::gl glew_s glm::glm OpenGL::GL
BulletDynamics BulletCollision LinearMath)

# set( INSTALL_PATH ${PROJECT_BINARY_DIR}/install )
# install( DIRECTORY resources DESTINATION ${INSTALL_PATH} )
# install( TARGETS ProjectMaricaCarGenerator
#         DESTINATION ${INSTALL_PATH} )
