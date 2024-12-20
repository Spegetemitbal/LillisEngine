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

# if (WIN32) remove System/System_Win32.cpp and System/System_Win32.h
IF(WIN32)
    set_target_properties(LillisLib PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    list(REMOVE_ITEM CORE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/System/System_Win32.cpp)
    list(REMOVE_ITEM CORE_INC ${CMAKE_CURRENT_SOURCE_DIR}/System/System_Win32.h)
ENDIF(WIN32)

find_package(OpenGL REQUIRED)

target_include_directories(LillisLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(
        LillisLib PUBLIC glm glfw glad
)

target_precompile_headers(LillisLib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/pch.h)
#set(CMAKE_PCH_INSTANTIATE_TEMPLATES ON)

target_compile_definitions(LillisLib PRIVATE LILLISENG)

install (TARGETS LillisLib DESTINATION lib)
install (FILES ${CORE_INC} DESTINATION include/core)