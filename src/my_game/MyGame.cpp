#include "my_game/MyGame.h"

#include <iostream>
#include <string>

namespace mygame {

void MyGame::OnInit(m2d::Engine& engine) {
    m_engine = &engine;
    m_input  = &engine.GetInput();

    auto& renderer = engine.GetRenderer();

    if (!m_playerSheet.LoadFromFile(renderer.Get(),
                                    "assets/textures/player_sheet.png")) {
        std::cerr << "[MyGame] Warning: player_sheet.png not loaded" << std::endl;
    }

    if (!m_font.LoadFromFile("assets/fonts/default.ttf", 24.0f)) {
        std::cerr << "[MyGame] Warning: default.ttf not loaded" << std::endl;
    }

    m_player.Init(&m_playerSheet);
    std::cout << "[MyGame] Initialized" << std::endl;
}

void MyGame::OnUpdate(float dt) {
    // FPS
    m_fpsTimer   += dt;
    m_frameCount += 1;
    if (m_fpsTimer >= 1.0f) {
        m_fps        = m_frameCount;
        m_frameCount = 0;
        m_fpsTimer  -= 1.0f;
    }

    // Esc — выход
    if (m_input->IsKeyPressed(SDL_SCANCODE_ESCAPE)) {
        m_engine->RequestQuit();
        return;
    }

    // Игрок
    m_player.Update(*m_input, dt);
}

void MyGame::OnRender(m2d::Renderer& renderer) {
    // Фон
    renderer.Clear(30, 30, 30);

    // Игрок
    m_player.Render(renderer);

    // FPS-счётчик
    std::string fpsText = "FPS: " + std::to_string(m_fps);
    renderer.DrawText(m_font, fpsText, 10.0f, 10.0f,
                      255, 255, 100, 255);

    // НЕ вызываем Present — это делает Engine после OnRender.
}

void MyGame::OnShutdown() {
    std::cout << "[MyGame] Shutdown" << std::endl;
}

} // namespace mygame