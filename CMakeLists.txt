cmake_minimum_required(VERSION 3.12)
project(opengl VERSION 1.0 LANGUAGES CXX C)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

add_subdirectory(dependencies)

add_executable(opengl
    src/main.cpp

    src/Camera.h
    src/Mesh.h
    src/Model.h
    src/Shader.h
)

target_link_libraries(opengl
    PRIVATE
        glfw
        glad
        assimp
        glm
        stb
)

target_link_libraries(opengl
    PRIVATE
        OpenGL::GL
)

set_property(TARGET opengl PROPERTY CXX_STANDARD 20)
set_property(TARGET opengl PROPERTY C_STANDARD 23)

install(TARGETS opengl DESTINATION bin)
install(DIRECTORY resources DESTINATION bin)

add_custom_command(TARGET opengl PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/resources
		$<TARGET_FILE_DIR:opengl>/resources)
