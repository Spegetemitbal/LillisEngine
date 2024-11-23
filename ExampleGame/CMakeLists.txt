file(
        GLOB_RECURSE
        EXAMPLE_INC
        ${CMAKE_CURRENT_SOURCE_DIR}
        *.h
)
file(
        GLOB_RECURSE
        EXAMPLE_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}
        *.cpp
)

add_custom_target(copyAssets ALL COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets/
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/)

add_executable(ExampleGame ${EXAMPLE_INC} ${EXAMPLE_SRC})
target_link_libraries(ExampleGame PUBLIC LillisLib glfw glad glm)
#target_include_directories(ExampleGame PUBLIC ${CORE_INC_DIR})

add_dependencies(ExampleGame copyAssets)
