#include "my_game/MyGame.h"

#include <iostream>

namespace mygame {

void MyGame::OnInit(m2d::Engine& engine) {
    m_engine = &engine;
    m_input  = &engine.GetInput();

    auto& resources = engine.GetResources();
    auto* sdlRenderer = engine.GetSDLRenderer();

    // Загрузка через ResourceManager — при повторном запросе вернётся кеш
    m_playerSheet = resources.GetTexture(sdlRenderer,
                                         "assets/textures/player_sheet.png");
    if (!m_playerSheet) {
        std::cerr << "[MyGame] Warning: player_sheet.png not loaded" << std::endl;
    }

    m_font = resources.GetFont("assets/fonts/default.ttf", 20.0f);
    if (!m_font) {
        std::cerr << "[MyGame] Warning: default.ttf not loaded" << std::endl;
    }

    // Привязываем шрифт к DebugOverlay движка
    if (m_font) {
        engine.GetDebugOverlay().Init(m_font);
    }

    // Передаём игроку текстуру — он тоже не владеет
    m_player.Init(m_playerSheet);

    std::cout << "[MyGame] Initialized (textures: "
              << resources.GetTextureCount()
              << ", fonts: " << resources.GetFontCount() << ")" << std::endl;
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