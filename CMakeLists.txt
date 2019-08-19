cmake_minimum_required(VERSION 3.12)
project(opengl VERSION 1.0 LANGUAGES CXX C)

set(OpenGL_GL_PREFERENCE GLVND)
set(CMAKE_AUTOMOC ON)

#find_package(OpenGL)

find_package(Qt5 COMPONENTS Core Gui REQUIRED)

add_subdirectory(dependencies)

add_executable(opengl
    src/main.cpp

    src/TestOpenGLWindow.cpp
    src/Model.cpp
    src/Shader.cpp
)

# I have to manually get the include directories from
# the assimp target because they use `include_directories()`
# instead of `target_include_directories(PUBLIC)`...
# which would've allowed cmake to automatically include them
# when you link against assimp in `target_link_libraries()`
# I should fix that in the CMakeLists.txt for assimp
get_target_property(assimp_INCLUDES assimp INCLUDE_DIRECTORIES)
target_include_directories(opengl PRIVATE ${assimp_INCLUDES})

target_link_libraries(opengl
    PRIVATE
        assimp
        Qt5::Core
        Qt5::Gui
)

install(TARGETS opengl DESTINATION bin)
install(DIRECTORY resources DESTINATION bin)

add_custom_command(TARGET opengl POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/resources
		$<TARGET_FILE_DIR:opengl>/resources)
