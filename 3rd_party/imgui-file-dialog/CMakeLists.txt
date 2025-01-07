project( imgui-file-dialog )
set( IMGUI_FILE_DIALOG_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/ImGuiFileDialog-0.6.7 )

if(NOT EXISTS ${IMGUI_FILE_DIALOG_ROOT})
	file(DOWNLOAD https://github.com/aiekick/ImGuiFileDialog/archive/refs/tags/v0.6.7.zip ${CMAKE_CURRENT_BINARY_DIR}/imgui-file-dialog.zip)

	file(ARCHIVE_EXTRACT INPUT ${CMAKE_CURRENT_BINARY_DIR}/imgui-file-dialog.zip
	DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})
endif()

add_library(imgui-file-dialog 
    ${IMGUI_FILE_DIALOG_ROOT}/ImGuiFileDialog.cpp)
add_library(imgui::file-dialog ALIAS imgui-file-dialog )
target_link_libraries(imgui-file-dialog imgui::core)
target_include_directories(imgui-file-dialog PUBLIC ${IMGUI_FILE_DIALOG_ROOT})
