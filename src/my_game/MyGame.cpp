#include "my_game/MyGame.h"
#include "core/Log.h"

namespace mygame {

void MyGame::OnInit(m2d::Engine& engine) {
    m_engine = &engine;
    m_input  = &engine.GetInput();

    auto& resources   = engine.GetResources();
    auto* sdlRenderer = engine.GetSDLRenderer();

    m_playerSheet = resources.GetTexture(sdlRenderer,
                                         "assets/textures/player_sheet.png");
    if (!m_playerSheet) {
        M2D_WARN("player_sheet.png not loaded");
    }

    m_font = resources.GetFont("assets/fonts/default.ttf", 20.0f);
    if (!m_font) {
        M2D_WARN("default.ttf not loaded");
    }

    if (m_font) {
        engine.GetDebugOverlay().Init(m_font);
    }

    m_player.Init(m_playerSheet);

    M2D_INFO("MyGame initialized (textures: ",
             resources.GetTextureCount(),
             ", fonts: ", resources.GetFontCount(), ")");
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
    M2D_INFO("MyGame shutdown");
}

} // namespace mygame