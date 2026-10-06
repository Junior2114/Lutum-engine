cmake_minimum_required(VERSION 3.16)
project(2DEngine CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE BOOL "" FORCE)

add_subdirectory(vendored/SDL       EXCLUDE_FROM_ALL)
add_subdirectory(vendored/SDL_image EXCLUDE_FROM_ALL)
add_subdirectory(vendored/SDL_ttf   EXCLUDE_FROM_ALL)

add_executable(2DEngine WIN32
    src/main.cpp
    src/core/Engine.cpp
    src/core/Input.cpp
    src/core/Log.cpp
    src/core/SceneManager.cpp
    src/ui/UIPanel.cpp
    src/ui/UILabel.cpp
    src/ui/UIButton.cpp
    src/ui/UISlider.cpp
    src/ui/UITextInput.cpp
    src/ui/UISeparator.cpp
    src/ui/UILayer.cpp
    src/graphics/Texture.cpp
    src/graphics/Renderer.cpp
    src/graphics/Animation.cpp
    src/graphics/Animator.cpp
    src/graphics/Font.cpp
    src/graphics/DebugOverlay.cpp
    src/graphics/ResourceManager.cpp
    src/game/Player.cpp
    src/game/Shape.cpp
    src/my_game/MyGame.cpp
    src/my_game/PlayScene.cpp
)

target_include_directories(2DEngine PRIVATE src)

target_link_libraries(2DEngine PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
)

if (WIN32)
    target_link_libraries(2DEngine PRIVATE psapi)
endif()