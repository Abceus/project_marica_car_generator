cmake_minimum_required( VERSION 3.31 )

project( ProjectMaricaCarGenerator )

set( CMAKE_CXX_STANDARD 20 )

set(BUILD_SHARED_LIBS OFF)

include(FetchContent)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG        3.4
        FIND_PACKAGE_ARGS
)
FetchContent_MakeAvailable(glfw)

add_subdirectory(3rd_party/imgui)
add_subdirectory(3rd_party/imgui-file-dialog)
add_subdirectory(3rd_party/stb)

set(LIBTYPE STATIC) #OpenAL
set(ALSOFT_UTILS OFF CACHE BOOL "Build utility programs" FORCE)
FetchContent_Declare(
        OpenAL
        GIT_REPOSITORY https://github.com/kcat/openal-soft.git
        GIT_TAG        1.24.2
        FIND_PACKAGE_ARGS
)
FetchContent_MakeAvailable(OpenAL)

FetchContent_Declare(
        SndFile
        GIT_REPOSITORY https://github.com/libsndfile/libsndfile.git
        GIT_TAG        1.2.2
        FIND_PACKAGE_ARGS
)
FetchContent_MakeAvailable(SndFile)

set(SOURCES sources/main.cpp

        sources/widgets/opengl_glfw_window.cpp
        sources/widgets/app.cpp
        sources/widgets/mainwindow.cpp
        sources/widgets/openglview.cpp
        sources/widgets/configuration_widget.cpp
        sources/widgets/file_picker_widget.cpp
        sources/widgets/float_input.cpp
        sources/widgets/sound_player.cpp
        sources/widgets/bone_selector_widget.cpp

        sources/sound/context.cpp
        sources/sound/sound.cpp

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
        sources/render_system/color.cpp
        sources/render_system/transform.cpp

        sources/render_system/camera/free_camera_controller.cpp
        
        sources/resources/model.cpp
        sources/resources/wireframe_model.cpp
        sources/resources/unanimation.cpp
        sources/resources/resource_manager.cpp
        sources/resources/skeletal.cpp
        sources/resources/dds_info.cpp
        sources/resources/image_info.cpp

        sources/resources/ase_reader/ase_reader_node.cpp
        sources/resources/ase_reader/ase_reader_node_analyzer.cpp
        sources/resources/ase_reader/ase_reader.cpp
        sources/resources/ase_reader/ase_tokenizer.cpp
        sources/resources/ase_reader/rules/lambda.cpp
        sources/resources/ase_reader/rules/int.cpp
        sources/resources/ase_reader/rules/string.cpp
        sources/resources/ase_reader/rules/dict.cpp
        sources/resources/ase_reader/rules/float.cpp
        sources/resources/ase_reader/rules/set.cpp
        sources/resources/ase_reader/rules/value.cpp
        
        sources/utils/math/angle.cpp
        sources/utils/math/rot3.cpp
        sources/utils/math/utils.cpp
        sources/utils/math/quaternion.cpp
        sources/utils/math/geometry_prototype.cpp
        sources/utils/math/line.cpp
        sources/utils/math/plane.cpp

        sources/utils/shapes/box.cpp
        sources/utils/shapes/sphere.cpp
        sources/utils/shapes/convex_hull.cpp)

set(PHYSIC_SOURCES
        sources/physics/physicworld.cpp
        sources/physics/physobject.cpp)

if(WITH_PHYSICS)
        set(SOURCES ${SOURCES} ${PHYSIC_SOURCES})
endif()

add_executable( ProjectMaricaCarGenerator
        ${SOURCES})

# set(ALSOFT_STD_VERSION_PROPS
#     # Require C++17.
#     CXX_STANDARD 17
#     CXX_STANDARD_REQUIRED TRUE

find_package(OpenGL REQUIRED)

set(GLEW_USE_STATIC_LIBS TRUE)
find_package(GLEW QUIET)
if(NOT GLEW_FOUND)
        add_subdirectory(3rd_party/glew)
endif()

FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG        bf71a834948186f4097caa076cd2663c69a10e1e
        FIND_PACKAGE_ARGS
)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG        v3.11.3
        FIND_PACKAGE_ARGS
)
FetchContent_MakeAvailable(nlohmann_json)

if(WITH_PHYSICS)
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

        FetchContent_Declare(
                Bullet
                GIT_REPOSITORY https://github.com/bulletphysics/bullet3.git
                GIT_TAG        7dee3436e747958e7088dfdcea0e4ae031ce619e
                FIND_PACKAGE_ARGS
        )
        FetchContent_MakeAvailable(Bullet)

        target_include_directories( ProjectMaricaCarGenerator
                PRIVATE ${BULLET_PHYSICS_SOURCE_DIR}/src)

        target_link_libraries(ProjectMaricaCarGenerator BulletDynamics BulletCollision LinearMath)

        target_compile_definitions(ProjectMaricaCarGenerator PUBLIC WITH_PHYSICS)
endif()

target_include_directories( ProjectMaricaCarGenerator
        PRIVATE includes
        PRIVATE ${GLEW_INCLUDE_DIRS})

target_link_libraries(ProjectMaricaCarGenerator 
        glew_s 
        glm::glm 
        OpenGL::GL 
        imgui::core 
        imgui::glfw 
        imgui::opengl3 
        glfw
        imgui::file-dialog
        stb::image
        nlohmann_json::nlohmann_json
        OpenAL::OpenAL
        sndfile)

# set( INSTALL_PATH ${PROJECT_BINARY_DIR}/install )
# install( DIRECTORY resources DESTINATION ${INSTALL_PATH} )
# install( TARGETS ProjectMaricaCarGenerator
#         DESTINATION ${INSTALL_PATH} )
