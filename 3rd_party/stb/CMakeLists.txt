project(stb)

set(STD_IMAGE_HEADER_FILE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/stb_image.h)

if(NOT EXISTS ${STD_IMAGE_HEADER_FILE_PATH})
	file(DOWNLOAD https://raw.githubusercontent.com/nothings/stb/refs/heads/master/stb_image.h ${STD_IMAGE_HEADER_FILE_PATH})
endif()

add_library(stb-image INTERFACE)
add_library(stb::image ALIAS stb-image)
target_include_directories(stb-image INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
