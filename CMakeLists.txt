cmake_minimum_required(VERSION 3.22 FATAL_ERROR)
project(graphics)

set(CMAKE_CXX_STANDARD 11)

if (NOT DEFINED ENV{GLFW_HOME})
    message(FATAL_ERROR "found no env named GLFW_HOME")
endif()
if (NOT DEFINED ENV{ASSIMP_HOME})
    message(FATAL_ERROR "found no env named ASSIMP_HOME")
endif()
if (NOT DEFINED ENV{GLM_HOME})
    message(FATAL_ERROR "found no env named GLM_HOME")
endif()

if(APPLE)
    set(GLFW_HOME $ENV{GLFW_HOME})
    set(ASSIMP_HOME ${CMAKE_CURRENT_SOURCE_DIR}/libs/APPLE/assimp)
elseif(WIN32)
    set(GLFW_HOME ${CMAKE_CURRENT_SOURCE_DIR}/libs/WIN32/glfw-3.3.9)
    set(ASSIMP_HOME ${CMAKE_CURRENT_SOURCE_DIR}/libs/WIN32/assimp)
endif()

set(GLAD_HOME ${CMAKE_CURRENT_SOURCE_DIR}/libs/glad)
set(GLM_HOME ${CMAKE_CURRENT_SOURCE_DIR}/libs/glm)
set(IMGUI_HOME ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui)

include_directories("${GLFW_HOME}/include")
include_directories("${GLAD_HOME}/include")
include_directories("${ASSIMP_HOME}/include")
include_directories("${GLM_HOME}/include")

message("GLFW_HOME: ${GLFW_HOME}")
message("GLAD_HOME: ${GLAD_HOME}")
message("ASSIMP_HOME: ${ASSIMP_HOME}")
message("GLM_HOME: ${GLM_HOME}")

# Reference asset directory
set(ASSET_DIR "${PROJECT_SOURCE_DIR}/assets")
add_definitions(-DASSET_DIR="${ASSET_DIR}")

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
add_definitions(-DSRC_DIR="${SRC_DIR}")

add_library(glfw SHARED IMPORTED)
if (APPLE)
    SET_TARGET_PROPERTIES(glfw PROPERTIES IMPORTED_LOCATION "${GLFW_HOME}/lib/libglfw.3.dylib")
elseif (WIN32)
    SET_TARGET_PROPERTIES(glfw PROPERTIES 
        IMPORTED_LOCATION "${GLFW_HOME}/lib-vc2022/glfw3.dll"
        IMPORTED_IMPLIB "${GLFW_HOME}/lib-vc2022/glfw3.lib")
endif()

add_library(assimp SHARED IMPORTED)
if (APPLE)
    SET_TARGET_PROPERTIES(assimp PROPERTIES IMPORTED_LOCATION "${ASSIMP_HOME}/bin/libassimp.5.dylib")
elseif (WIN32)
    SET_TARGET_PROPERTIES(assimp PROPERTIES 
        IMPORTED_LOCATION "${ASSIMP_HOME}/bin/assimp-vc143-mtd.dll"
        IMPORTED_IMPLIB "${ASSIMP_HOME}/lib/assimp/assimp-vc143-mtd.lib")
endif()

add_library(glm INTERFACE)

add_library(glad STATIC "${GLAD_HOME}/src/glad.c")

add_subdirectory("${IMGUI_HOME}")

add_executable(graphics
    main.cpp
    src/shader.h
    src/shader.cpp
    src/entity.h
    src/gameObject.h
    src/material.h
    src/texture.h
    src/mesh.h
    src/entityModule.h
    src/model.h
    src/model.cpp
    src/lights.h
    src/resourceManager.h
    src/resourceManager.cpp
    src/entityModules/renderModule.h
    src/entityModules/gameplayModule.h
    src/entityModules/camera.h
    src/entityModules/controllerModule.h
    src/shaders/forwardPass/forwardShader.h
    src/shaders/forwardPass/blinnPhongShader.h
    src/shaders/forwardPass/toonShader.h
    src/shaders/forwardPass/pbrShader.h
    src/shaders/forwardPass/testShader.h
    src/utils/stb_image.h
    src/utils/stb_image.cpp
    src/utils/animData.h
    src/utils/assimpHelper.h
    src/utils/programInfo.h
    src/imgui/imguiWrapper.h
    src/imgui/imguiWrapper.cpp
    src/materials/PBRMaterial.h
    src/materials/basicMaterial.h
    demos/rendering1.h
    )

if (APPLE)
    target_link_libraries(graphics glfw glad assimp glm imgui "-framework OpenGL")
elseif (WIN32)
    target_link_libraries(graphics glfw glad assimp glm imgui opengl32) # Link to opengl32 on Windows
endif()
