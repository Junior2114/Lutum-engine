cmake_minimum_required(VERSION 3.16)
project(2DEngine CXX)

# ===== C++ стандарт =====
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ===== Куда класть бинарники =====
# Все .exe и .dll попадают в build/<Config>/ (Debug, Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# ===== Подавляем dev-предупреждения от сторонних библиотек =====
# SDL_image / libtiff / libwebp сыпят warning'ами для своих разработчиков —
# нам они не нужны.
set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE BOOL "" FORCE)

# ===== Подпроекты (vendored dependencies) =====
# Порядок ВАЖЕН: SDL идёт первым, потому что от него зависят остальные.
add_subdirectory(vendored/SDL       EXCLUDE_FROM_ALL)
add_subdirectory(vendored/SDL_image EXCLUDE_FROM_ALL)
add_subdirectory(vendored/SDL_ttf   EXCLUDE_FROM_ALL)

# ===== Наш исполняемый файл =====
add_executable(2DEngine WIN32
    src/main.cpp
    src/core/Application.cpp
    src/core/Input.cpp
    src/graphics/Texture.cpp
    src/graphics/Renderer.cpp
    src/graphics/Animation.cpp
    src/graphics/Animator.cpp
    src/graphics/Font.cpp
    src/game/Player.cpp
)

# ===== Include paths =====
# Благодаря этому пишем #include "core/Application.h",
# а не #include "../../src/core/Application.h"
target_include_directories(2DEngine PRIVATE src)

# ===== Линковка библиотек =====
target_link_libraries(2DEngine PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
)