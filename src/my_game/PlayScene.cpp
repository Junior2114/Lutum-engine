#include "my_game/PlayScene.h"
#include "my_game/MyGame.h"
#include "core/Log.h"

namespace mygame {

void PlayScene::OnEnter(m2d::Engine& engine) {
    m_engine = &engine;
    m_input  = &engine.GetInput();

    auto& resources   = engine.GetResources();
    auto* sdlRenderer = engine.GetSDLRenderer();

    m_playerSheet = resources.GetTexture(sdlRenderer,
                                         "assets/textures/player_sheet.png");
    M2D_ASSERT(m_playerSheet != nullptr, "Player sheet failed to load");

    m_font = resources.GetFont("assets/fonts/default.ttf", 20.0f);

    if (m_font) {
        engine.GetDebugOverlay().Init(m_font);
    }

    m_player.Init(m_playerSheet);

    M2D_INFO("PlayScene entered");
}

void PlayScene::OnExit() {
    M2D_INFO("PlayScene exited");
}

void PlayScene::OnUpdate(float dt) {
    if (m_input->IsKeyPressed(SDL_SCANCODE_ESCAPE)) {
        m_engine->RequestQuit();
        return;
    }

    m_player.Update(*m_input, dt);
}

void PlayScene::OnRender(m2d::Renderer& renderer) {
    renderer.Clear(30, 30, 30);
    m_player.Render(renderer);
}

} // namespace mygame