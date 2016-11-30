cmake_minimum_required(VERSION 3.5)
project(opengl)

find_package(OpenGL REQUIRED)

add_subdirectory(${CMAKE_SOURCE_DIR}/dependencies/glfw-3.2.1)
add_subdirectory(${CMAKE_SOURCE_DIR}/dependencies/glew-2.0.0/build/cmake)
add_subdirectory(${CMAKE_SOURCE_DIR}/dependencies/assimp-3.3.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(
	${CMAKE_SOURCE_DIR}/dependencies/glfw-3.2.1/include
	${CMAKE_SOURCE_DIR}/dependencies/glew-2.0.0/include
	${CMAKE_SOURCE_DIR}/dependencies/glm-0.9.8.2
    ${CMAKE_SOURCE_DIR}/dependencies/stb-master
	${CMAKE_SOURCE_DIR}/dependencies/assimp-3.3.1/include
	${CMAKE_SOURCE_DIR}/dependencies/assimp-3.3.1/code
    ${OPENGL_INCLUDE_DIR})

set(SOURCE_FILES 
    main.cpp
	Shader.h
	Camera.h)

add_executable(opengl ${SOURCE_FILES})

target_link_libraries(opengl glfw ${GLFW_LIBRARIES})
target_link_libraries(opengl glew_s)
target_link_libraries(opengl assimp)
if(OPENGL_FOUND)
    target_link_libraries(opengl ${OPENGL_gl_LIBRARY})
endif()
if(OPENGL_GLU_FOUND)
    target_link_libraries(opengl ${OPENGL_glu_LIBRARY})
endif()

install(TARGETS opengl DESTINATION bin)
install(DIRECTORY resources DESTINATION bin)

add_custom_command(TARGET opengl PRE_BUILD	
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/resources
		$<TARGET_FILE_DIR:opengl>/resources)
