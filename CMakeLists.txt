cmake_minimum_required(VERSION 3.16)
project(2DEngine CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# SDL как подпроект
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

# Все исходники движка
add_executable(2DEngine WIN32
    src/main.cpp
    src/core/Application.cpp
)

# Чтобы #include "core/Application.h" работал
target_include_directories(2DEngine PRIVATE src)

target_link_libraries(2DEngine PRIVATE SDL3::SDL3)