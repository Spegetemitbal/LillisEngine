#GLFW
string(TIMESTAMP BEFORE "%s")

CPMAddPackage(
        NAME "glfw"
        URL "https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip"
        OPTIONS ("GLFW_BUILD_EXAMPLES OFF" "GLFW_BUILD_TESTS OFF" "GLFW_BUILD_DOCS OFF")
)
find_package(glfw REQUIRED)
set (glfw_INCLUDE_DIR ${glfw_SOURCE_DIR}/include)
include_directories(${glfw_INCLUDE_DIR})
string(TIMESTAMP AFTER "%s")
math(EXPR DELTAglfw "${AFTER}-${BEFORE}")
MESSAGE(STATUS "glfw TIME: ${DELTAglfw}s")