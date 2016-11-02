cmake_minimum_required(VERSION 3.5)
project(opengl)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(
    dependencies/glfw/include
    ${OPENGL_INCLUDE_DIR})

set(SOURCE_FILES 
    main.cpp)

add_executable(opengl ${SOURCE_FILES})

target_link_libraries(opengl glfw)
if(OPENGL_FOUND)
    target_link_libraries(opengl ${OPENGL_gl_LIBRARY})
endif()
if(OPENGL_GLU_FOUND)
    target_link_libraries(opengl ${OPENGL_glu_LIBRARY})
endif()
