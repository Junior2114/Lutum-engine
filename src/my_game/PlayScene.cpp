#include "my_game/PlayScene.h"
#include "my_game/MyGame.h"
#include "core/Log.h"

namespace mygame {

void PlayScene::OnEnter(m2d::Engine& engine) {
    m_engine = &engine;
    m_input  = &engine.GetInput();

    auto& resources   = engine.GetResources();
    auto* sdlRenderer = engine.GetSDLRenderer();

    // ===== Ресурсы =====
    m_playerSheet = resources.GetTexture(sdlRenderer,
                                         "assets/textures/player_sheet.png");
    M2D_ASSERT(m_playerSheet != nullptr, "Player sheet failed to load");

    m_font = resources.GetFont("assets/fonts/default.ttf", 20.0f);
    if (m_font) {
        engine.GetDebugOverlay().Init(m_font);
    }

    m_player.Init(m_playerSheet);

    // ===== UI-панель слева =====
    auto& ui = engine.GetUI();
    auto* uiFont = ui.GetFont();

    constexpr float PANEL_W = 220.0f;

    auto* panel = ui.Add<m2d::UIPanel>();
    panel->SetPosition(0.0f, 0.0f);
    panel->SetSize(PANEL_W, (float)engine.GetHeight());
    panel->SetBackgroundColor(20, 20, 25, 240);
    panel->SetBorder(true, 60, 60, 80);

    // Заголовок
    auto* title = panel->AddChild<m2d::UILabel>();
    title->SetPosition(16.0f, 16.0f);
    title->SetFont(uiFont);
    title->SetText("Tools");
    title->SetColor(180, 180, 220);

    // Кнопки-заглушки
    const char* buttonLabels[] = { "Rectangle", "Circle", "Triangle" };
    float y = 60.0f;
    for (const char* label : buttonLabels) {
        auto* btn = panel->AddChild<m2d::UIButton>();
        btn->SetPosition(16.0f, y);
        btn->SetSize(PANEL_W - 32.0f, 36.0f);
        btn->SetFont(uiFont);
        btn->SetText(label);
        y += 46.0f;
    }

    M2D_INFO("PlayScene entered (UI panel added)");
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