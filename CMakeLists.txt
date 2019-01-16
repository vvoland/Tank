cmake_minimum_required(VERSION 3.5)
project(Tank)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_VERBOSE_MAKEFILE 1)
set(APP_VERSION_MAJOR 1)
set(APP_VERSION_MINOR 0)

set(APP_TARGET Tank)
set(OpenGL_GL_PREFERENCE LEGACY)



include_directories(${PROJECT_SOURCE_DIR})
set(PROJECT_SOURCE_FILES
        "main.cpp"
        "app.cpp"
        "draw_call.cpp"
        "draw_call_builder.cpp"
        "gameobject.cpp"
        "material.cpp"
        "mesh.cpp"
        "shader.cpp"
        "shader_program.cpp"
        "tank_app.cpp"
        "app_assets.cpp"
        "texture.cpp"
        "vao.cpp"
        "vbo.cpp"
        )

include_directories(${PROJECT_SOURCE_DIR}/components)
set(PROJECT_SOURCE_FILES
        ${PROJECT_SOURCE_FILES}
        "components/renderer.cpp"
        "components/transform.cpp"
        "components/velocity.cpp"
        "components/camera.cpp"
        components/light.cpp components/light.hpp)

include_directories(${PROJECT_SOURCE_DIR}/systems)
set(PROJECT_SOURCE_FILES
        ${PROJECT_SOURCE_FILES}
        "systems/input_system.cpp"
        )

add_executable(${APP_TARGET} ${PROJECT_SOURCE_FILES})

# Set higher warning level
if(MSVC)
    target_compile_options(${APP_TARGET} PRIVATE /W4 /WX)
else(MSVC)
    target_compile_options(${APP_TARGET} PRIVATE -Wall -Wextra -pedantic)
endif(MSVC)

# Dependencies
include_directories(${PROJECT_SOURCE_DIR}/external)

# imgui
add_subdirectory(external/imgui)
target_link_libraries(${APP_TARGET} imgui)

# stb_image
add_subdirectory(external/stb_image)
target_link_libraries(${APP_TARGET} stb_image)

# glfw
find_package(glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})
link_libraries(${GLFW_LIBRARY_DIRS})

# opengl
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# glew
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

# assimp
find_package(assimp REQUIRED)
include_directories(${ASSIMP_INCLUDE_DIRS})

if (APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(OpenGL_LIBRARY OpenGL)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COREVIDEO_LIBRARY CoreVideo)
    SET(EXTRA_LIBS ${COCOA_LIBRARY} ${OpenGL_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY})
endif (APPLE)

if (WIN32)
# nothing now
endif (WIN32)

target_link_libraries(
        ${APP_TARGET}
        glfw
        ${OPENGL_LIBRARIES}
        ${GLEW_LIBRARIES}
        ${ASSIMP_LIBRARIES}
        ${EXTRA_LIBS}
)
