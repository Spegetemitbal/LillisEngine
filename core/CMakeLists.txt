file(
        GLOB_RECURSE
        CORE_INC
        ${CMAKE_CURRENT_SOURCE_DIR}
        *.h
)
file(
        GLOB_RECURSE
        CORE_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}
        *.cpp
)

add_library(LillisLib SHARED ${CORE_INC} ${CORE_SRC})

find_package(OpenGL REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(
        LillisLib PUBLIC glm
)

target_precompile_headers(LillisLib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/pch.h)
#set(CMAKE_PCH_INSTANTIATE_TEMPLATES ON)

add_compile_definitions(LILLISENG)

install (TARGETS LillisLib DESTINATION lib)
install (FILES ${CORE_INC} DESTINATION include/core)