#include "my_game/MyGame.h"

#include <iostream>

namespace mygame {

void MyGame::OnInit(m2d::Engine& engine) {
    m_engine = &engine;
    m_input  = &engine.GetInput();

    auto& renderer = engine.GetRenderer();

    if (!m_playerSheet.LoadFromFile(renderer.Get(),
                                    "assets/textures/player_sheet.png")) {
        std::cerr << "[MyGame] Warning: player_sheet.png not loaded" << std::endl;
    }

    if (!m_font.LoadFromFile("assets/fonts/default.ttf", 20.0f)) {
        std::cerr << "[MyGame] Warning: default.ttf not loaded" << std::endl;
    }

    engine.GetDebugOverlay().Init(&m_font);

    m_player.Init(&m_playerSheet);

    std::cout << "[MyGame] Initialized" << std::endl;
}

void MyGame::OnUpdate(float dt) {
    if (m_input->IsKeyPressed(SDL_SCANCODE_ESCAPE)) {
        m_engine->RequestQuit();
        return;
    }

    m_player.Update(*m_input, dt);
}

void MyGame::OnRender(m2d::Renderer& renderer) {
    renderer.Clear(30, 30, 30);
    m_player.Render(renderer);
}

void MyGame::OnShutdown() {
    std::cout << "[MyGame] Shutdown" << std::endl;
}

} // namespace mygame