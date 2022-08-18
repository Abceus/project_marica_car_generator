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
        sources/render_system/vertex_array.cpp
        sources/render_system/vertex_buffer.cpp
        sources/render_system/vertex_shader.cpp
        sources/render_system/shader_program.cpp
        sources/render_system/mesh.cpp
        sources/render_system/camera.cpp
        sources/render_system/camera_manager.cpp
        sources/render_system/scene.cpp
        sources/render_system/scene_node.cpp

        sources/resources/model.cpp
        sources/resources/unanimation.cpp)

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

target_include_directories( ProjectMaricaCarGenerator
        PRIVATE includes)

target_link_libraries(ProjectMaricaCarGenerator wx::core wx::base wx::gl glew_s glm::glm OpenGL::GL)

# set( INSTALL_PATH ${PROJECT_BINARY_DIR}/install )
# install( DIRECTORY resources DESTINATION ${INSTALL_PATH} )
# install( TARGETS ProjectMaricaCarGenerator
#         DESTINATION ${INSTALL_PATH} )
