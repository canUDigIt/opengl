cmake_minimum_required(VERSION 3.12)

include(FetchContent)
FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp
    GIT_TAG v4.1.0
)

FetchContent_GetProperties(assimp)
if(NOT assimp_POPULATED)
    FetchContent_Populate(assimp)
    set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "")
    add_subdirectory(${assimp_SOURCE_DIR} ${assimp_BINARY_DIR})
endif()
