project( imgui )
set( IMGUI_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/imgui-1.91.6 )

if(NOT EXISTS ${IMGUI_ROOT})
	file(DOWNLOAD https://github.com/ocornut/imgui/archive/refs/tags/v1.91.6.zip ${CMAKE_CURRENT_BINARY_DIR}/imgui.zip)

	file(ARCHIVE_EXTRACT INPUT ${CMAKE_CURRENT_BINARY_DIR}/imgui.zip
	DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})
endif()

add_library(imgui-core 
    ${IMGUI_ROOT}/imgui.cpp 
    ${IMGUI_ROOT}/imgui_draw.cpp
    ${IMGUI_ROOT}/imgui_widgets.cpp
    ${IMGUI_ROOT}/imgui_tables.cpp
    ${IMGUI_ROOT}/imgui_demo.cpp)
add_library(imgui::core ALIAS imgui-core)
target_include_directories(imgui-core PUBLIC ${IMGUI_ROOT})

add_library(imgui-glfw ${IMGUI_ROOT}/backends/imgui_impl_glfw.cpp)
add_library(imgui::glfw ALIAS imgui-glfw)
target_link_libraries(imgui-glfw imgui-core glfw)
target_include_directories(imgui-glfw PUBLIC ${IMGUI_ROOT}/backends)

add_library(imgui-opengl3 ${IMGUI_ROOT}/backends/imgui_impl_opengl3.cpp)
add_library(imgui::opengl3 ALIAS imgui-opengl3)
target_link_libraries(imgui-opengl3 imgui-core)
target_include_directories(imgui-opengl3 PUBLIC ${IMGUI_ROOT}/backends)
