if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/glew-2.2.0)
	file(DOWNLOAD https://github.com/nigels-com/glew/releases/download/glew-2.2.0/glew-2.2.0.zip ${CMAKE_CURRENT_BINARY_DIR}/glew.zip)

	file(ARCHIVE_EXTRACT INPUT ${CMAKE_CURRENT_BINARY_DIR}/glew.zip
	DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/glew-2.2.0/build/cmake)

set( GLEW_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/glew-2.2.0/include PARENT_SCOPE )