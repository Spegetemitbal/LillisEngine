cmake_minimum_required(VERSION 3.16)

include(cmake/get_cpm.cmake)


project(
        Lillis
        VERSION 0.0.1
        LANGUAGES CXX C
)

#set C++ version to 23
set(CMAKE_CXX_STANDARD 23)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

link_directories(${CMAKE_BINARY_DIR}/bin)

include(External/glm.cmake)
include(External/glfw.cmake)
include(External/stb.cmake)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/External/glad/include)
file(GLOB BUTTERFLIES_SOURCES_C ${CMAKE_CURRENT_SOURCE_DIR} *.c External/glad/src/gl.c)

add_subdirectory(core)
add_subdirectory(ExampleGame)