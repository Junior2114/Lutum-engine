#include "my_game/PlayScene.h"
#include "my_game/MyGame.h"
#include "core/Log.h"

#include <random>
#include <algorithm>

namespace mygame {

static float RandomFloat(float min, float max) {
    static std::mt19937 gen{ std::random_device{}() };
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}

void PlayScene::OnEnter(m2d::Engine& engine) {
    m_engine = &engine;
    m_input  = &engine.GetInput();

    auto& resources = engine.GetResources();

    m_font = resources.GetFont("assets/fonts/default.ttf", 20.0f);
    if (m_font) {
        engine.GetDebugOverlay().Init(m_font);
    }

    // ===== UI-панель =====
    auto& ui     = engine.GetUI();
    auto* uiFont = ui.GetFont();

    constexpr float PANEL_W = 220.0f;

    auto* panel = ui.Add<m2d::UIPanel>();
    panel->SetPosition(0.0f, 0.0f);
    panel->SetSize(PANEL_W, (float)engine.GetHeight());
    panel->SetBackgroundColor(20, 20, 25, 240);
    panel->SetBorder(true, 60, 60, 80);

    auto* title = panel->AddChild<m2d::UILabel>();
    title->SetPosition(16.0f, 16.0f);
    title->SetFont(uiFont);
    title->SetText("Tools");
    title->SetColor(180, 180, 220);

    struct ButtonDef {
        const char* label;
        m2d::Shape::Type type;
    };

    ButtonDef defs[] = {
        { "Rectangle", m2d::Shape::Type::Rectangle },
        { "Circle",    m2d::Shape::Type::Circle    },
        { "Triangle",  m2d::Shape::Type::Triangle  },
    };

    float y = 60.0f;
    for (const auto& def : defs) {
        auto* btn = panel->AddChild<m2d::UIButton>();
        btn->SetPosition(16.0f, y);
        btn->SetSize(PANEL_W - 32.0f, 36.0f);
        btn->SetFont(uiFont);
        btn->SetText(def.label);

        const m2d::Shape::Type type = def.type;
        btn->SetOnClick([this, type]() {
            AddRandomShape(type);
        });

        y += 46.0f;
    }

    y += 20.0f;
    auto* clearBtn = panel->AddChild<m2d::UIButton>();
    clearBtn->SetPosition(16.0f, y);
    clearBtn->SetSize(PANEL_W - 32.0f, 36.0f);
    clearBtn->SetFont(uiFont);
    clearBtn->SetText("Clear");
    clearBtn->SetColors(120, 40, 40, 160, 60, 60, 90, 30, 30);
    clearBtn->SetOnClick([this]() {
        ClearShapes();
    });

    M2D_INFO("PlayScene entered");
}

void PlayScene::OnExit() {
    M2D_INFO("PlayScene exited");
}

void PlayScene::OnUpdate(float dt) {
    (void)dt;

    if (m_input->IsKeyPressed(SDL_SCANCODE_ESCAPE)) {
        m_engine->RequestQuit();
        return;
    }

    const float mx = m_input->GetMouseX();
    const float my = m_input->GetMouseY();

    const bool uiHovered = m_engine->GetUI().IsPointOverUI(mx, my);

    // ===== Обработка фигур — только если клик не над UI =====
    if (!uiHovered) {
        // Клик (нажатие) — toggle выделения
        if (m_input->IsMouseButtonPressed(SDL_BUTTON_LEFT)) {
            HandleShapeClick(mx, my);
        }

        // Drag — пока мышь зажата и есть выделенная фигура
        if (m_input->IsMouseButtonDown(SDL_BUTTON_LEFT)) {
            HandleShapeDrag(mx, my);
        }
    }

    // Отпускание мыши — заканчиваем drag
    if (m_input->IsMouseButtonReleased(SDL_BUTTON_LEFT)) {
        if (m_dragShape) {
            M2D_INFO("Stopped dragging shape");
            m_dragShape = nullptr;
        }
    }

    // Обновляем состояние кнопки мыши
    m_mouseWasDown = m_input->IsMouseButtonDown(SDL_BUTTON_LEFT);
}

void PlayScene::OnRender(m2d::Renderer& renderer) {
    renderer.Clear(30, 30, 30);

    for (const auto& shape : m_shapes) {
        shape.Render(renderer);
    }
}

// ===== Внутреннее =====

m2d::Shape* PlayScene::FindShapeAt(float x, float y) {
    // Ищем с конца — верхние (последние добавленные) фигуры в приоритете
    for (auto it = m_shapes.rbegin(); it != m_shapes.rend(); ++it) {
        if (it->ContainsPoint(x, y)) {
            return &(*it);
        }
    }
    return nullptr;
}

void PlayScene::DeselectAll() {
    for (auto& shape : m_shapes) {
        shape.SetSelected(false);
    }
}

void PlayScene::HandleShapeClick(float mx, float my) {
    m2d::Shape* hit = FindShapeAt(mx, my);

    if (!hit) {
        // Клик по пустому месту — снять выделение со всех
        DeselectAll();
        M2D_INFO("Deselected all");
        return;
    }

    if (hit->IsSelected()) {
        // Повторный клик по выделенной — снять выделение
        hit->SetSelected(false);
        m_dragShape = nullptr;   // не тащим её
        M2D_INFO("Deselected shape");
    } else {
        // Снять выделение со всех, выделить эту
        DeselectAll();
        hit->SetSelected(true);
        M2D_INFO("Selected shape");
    }
}

void PlayScene::HandleShapeDrag(float mx, float my) {
    // Если уже тащим — двигаем
    if (m_dragShape) {
        m_dragShape->MoveTo(mx - m_dragOffsetX, my - m_dragOffsetY);
        return;
    }

    // Если ещё не тащим — попробуем начать drag с выделенной фигуры под мышью
    m2d::Shape* hit = FindShapeAt(mx, my);
    if (hit && hit->IsSelected()) {
        m_dragShape = hit;
        m_dragOffsetX = mx - hit->GetX();
        m_dragOffsetY = my - hit->GetY();
        M2D_INFO("Started dragging shape");
    }
}

// ===== Фигуры =====

void PlayScene::AddRandomShape(m2d::Shape::Type type) {
    constexpr float PANEL_W = 220.0f;
    constexpr float MARGIN  = 60.0f;

    const int W = m_engine->GetWidth();
    const int H = m_engine->GetHeight();

    const float size = RandomFloat(40.0f, 100.0f);
    const float x = RandomFloat(PANEL_W + MARGIN, (float)W - size - MARGIN);
    const float y = RandomFloat(MARGIN, (float)H - size - MARGIN - 40.0f);

    static const Uint8 palette[][3] = {
        { 220,  80,  80 },
        {  80, 200, 120 },
        {  80, 140, 220 },
        { 220, 180,  80 },
        { 180,  80, 220 },
        {  80, 200, 200 },
    };
    const int paletteSize = sizeof(palette) / sizeof(palette[0]);
    const int idx = m_shapeCounter % paletteSize;
    ++m_shapeCounter;

    m_shapes.emplace_back(type, x, y, size, size,
                          palette[idx][0], palette[idx][1], palette[idx][2]);

    M2D_INFO("Added shape at (", (int)x, ", ", (int)y, ")");
}

void PlayScene::ClearShapes() {
    m_shapes.clear();
    m_shapeCounter = 0;
    m_dragShape = nullptr;
    M2D_INFO("Cleared all shapes");
}

} // namespace mygame