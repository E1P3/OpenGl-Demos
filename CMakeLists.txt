cmake_minimum_required(VERSION 3.15)
project(graphics)

set(CMAKE_CXX_STANDARD 11)

if (NOT DEFINED ENV{GLFW_HOME})
    message(FATAL_ERROR "found no env named GLFW_HOME")
endif()

set(GLFW_HOME $ENV{GLFW_HOME})
set(GLAD_HOME ${CMAKE_CURRENT_SOURCE_DIR}/libs/glad)
set(ASSIMP_HOME $ENV{ASSIMP_HOME})
set(GLM_HOME $ENV{GLM_HOME})

include_directories("${GLFW_HOME}/include")
include_directories("${GLAD_HOME}/include")
include_directories("${ASSIMP_HOME}/include")
include_directories("${GLM_HOME}/include")

add_library(glfw SHARED IMPORTED)
SET_TARGET_PROPERTIES(glfw PROPERTIES IMPORTED_LOCATION "${GLFW_HOME}/lib/libglfw.3.dylib")

add_library(glad STATIC "${GLAD_HOME}/src/glad.c")

add_library(assimp SHARED IMPORTED)
SET_TARGET_PROPERTIES(assimp PROPERTIES IMPORTED_LOCATION "${ASSIMP_HOME}/lib/libassimp.5.dylib")

add_library(glm INTERFACE)

add_executable(graphics main.cpp)

target_link_libraries(graphics glfw glad assimp glm "-framework OpenGL")