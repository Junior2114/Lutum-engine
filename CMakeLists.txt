cmake_minimum_required(VERSION 3.16)
project(2DEngine CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)
set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE BOOL "" FORCE)

# Подпроекты SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
add_subdirectory(vendored/SDL_image EXCLUDE_FROM_ALL)

add_executable(2DEngine WIN32
    src/main.cpp
    src/core/Application.cpp
    src/core/Input.cpp
    src/graphics/Texture.cpp
    src/graphics/Renderer.cpp
    src/graphics/Animation.cpp
    src/graphics/Animator.cpp
)
target_include_directories(2DEngine PRIVATE src)

target_link_libraries(2DEngine PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
)