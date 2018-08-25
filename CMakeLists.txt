cmake_minimum_required(VERSION 3.9)
project(opengl VERSION 1.0 LANGUAGES CXX C)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

add_subdirectory(${CMAKE_SOURCE_DIR}/dependencies/glfw-3.2.1)
add_subdirectory(${CMAKE_SOURCE_DIR}/dependencies/assimp-4.1.0)

add_executable(opengl
    src/main.cpp
    dependencies/glad/src/glad.c
)

target_include_directories(opengl
    PRIVATE
        include

        dependencies/glad/include
        dependencies/glfw-3.2.1/include
        dependencies/glm-0.9.8.2
        dependencies/stb-master
        dependencies/assimp-4.1.0/include
)

target_link_libraries(opengl
    PRIVATE
        glfw
        ${GLFW_LIBRARIES}
        assimp
        OpenGL::OpenGL
        OpenGL::GLX
        OpenGL::GLU
)

install(TARGETS opengl DESTINATION bin)
install(DIRECTORY resources DESTINATION bin)

add_custom_command(TARGET opengl PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/resources
		$<TARGET_FILE_DIR:opengl>/resources)
