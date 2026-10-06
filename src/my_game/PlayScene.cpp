#include "my_game/PlayScene.h"
#include "my_game/MyGame.h"
#include "core/Log.h"

#include <random>
#include <algorithm>
#include <string>

namespace mygame {

static float RandomFloat(float min, float max) {
    static std::mt19937 gen{ std::random_device{}() };
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}

// ===== OnEnter =====
void PlayScene::OnEnter(m2d::Engine& engine) {
    m_engine = &engine;
    m_input  = &engine.GetInput();

    auto& resources = engine.GetResources();

    m_font = resources.GetFont("assets/fonts/default.ttf", 20.0f);
    if (m_font) {
        engine.GetDebugOverlay().Init(m_font);
    }

    auto& ui     = engine.GetUI();
    auto* uiFont = ui.GetFont();

    const int W = engine.GetWidth();
    const int H = engine.GetHeight();

    // ===== Левая панель =====
    constexpr float LEFT_W = 220.0f;

    auto* leftPanel = ui.Add<m2d::UIPanel>();
    leftPanel->SetPosition(0.0f, 0.0f);
    leftPanel->SetSize(LEFT_W, (float)H);
    leftPanel->SetBackgroundColor(20, 20, 25, 240);
    leftPanel->SetBorder(true, 60, 60, 80);

    auto* title = leftPanel->AddChild<m2d::UILabel>();
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
        auto* btn = leftPanel->AddChild<m2d::UIButton>();
        btn->SetPosition(16.0f, y);
        btn->SetSize(LEFT_W - 32.0f, 36.0f);
        btn->SetFont(uiFont);
        btn->SetText(def.label);

        const m2d::Shape::Type type = def.type;
        btn->SetOnClick([this, type]() {
            AddRandomShape(type);
        });

        y += 46.0f;
    }

    y += 20.0f;
    auto* clearBtn = leftPanel->AddChild<m2d::UIButton>();
    clearBtn->SetPosition(16.0f, y);
    clearBtn->SetSize(LEFT_W - 32.0f, 36.0f);
    clearBtn->SetFont(uiFont);
    clearBtn->SetText("Clear");
    clearBtn->SetColors(120, 40, 40, 160, 60, 60, 90, 30, 30);
    clearBtn->SetOnClick([this]() {
        ClearShapes();
    });

    // ===== Правая панель — ИНСПЕКТОР =====
    constexpr float RIGHT_W = 260.0f;

    m_inspectorPanel = ui.Add<m2d::UIPanel>();
    m_inspectorPanel->SetPosition((float)W - RIGHT_W, 0.0f);
    m_inspectorPanel->SetSize(RIGHT_W, (float)H);
    m_inspectorPanel->SetBackgroundColor(20, 20, 25, 240);
    m_inspectorPanel->SetBorder(true, 60, 60, 80);
    m_inspectorPanel->SetVisible(false);   // пока ничего не выделено

    auto* inspTitle = m_inspectorPanel->AddChild<m2d::UILabel>();
    inspTitle->SetPosition(16.0f, 16.0f);
    inspTitle->SetFont(uiFont);
    inspTitle->SetText("Inspector");
    inspTitle->SetColor(180, 180, 220);

    // Type: Rectangle
    m_inspectorTypeLabel = m_inspectorPanel->AddChild<m2d::UILabel>();
    m_inspectorTypeLabel->SetPosition(16.0f, 60.0f);
    m_inspectorTypeLabel->SetFont(uiFont);
    m_inspectorTypeLabel->SetText("Type: -");
    m_inspectorTypeLabel->SetColor(220, 220, 220);

    // Scale: 1.00
    m_inspectorScaleLabel = m_inspectorPanel->AddChild<m2d::UILabel>();
    m_inspectorScaleLabel->SetPosition(16.0f, 110.0f);
    m_inspectorScaleLabel->SetFont(uiFont);
    m_inspectorScaleLabel->SetText("Scale: 1.00");
    m_inspectorScaleLabel->SetColor(220, 220, 220);

    // Слайдер Scale
    m_inspectorScaleSlider = m_inspectorPanel->AddChild<m2d::UISlider>();
    m_inspectorScaleSlider->SetPosition(16.0f, 145.0f);
    m_inspectorScaleSlider->SetSize(RIGHT_W - 32.0f, 20.0f);
    m_inspectorScaleSlider->SetRange(0.2f, 3.0f);
    m_inspectorScaleSlider->SetValue(1.0f);
    m_inspectorScaleSlider->SetOnChange([this](float value) {
        auto* shape = FindSelectedShape();
        if (shape) {
            shape->SetScale(value);
            // Обновляем текст
            char buf[64];
            snprintf(buf, sizeof(buf), "Scale: %.2f", value);
            m_inspectorScaleLabel->SetText(buf);
        }
    });

    M2D_INFO("PlayScene entered (with inspector)");
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

    if (!uiHovered) {
        if (m_input->IsMouseButtonPressed(SDL_BUTTON_LEFT)) {
            HandleShapeClick(mx, my);
        }

        if (m_input->IsMouseButtonDown(SDL_BUTTON_LEFT)) {
            HandleShapeDrag(mx, my);
        }
    }

    if (m_input->IsMouseButtonReleased(SDL_BUTTON_LEFT)) {
        if (m_dragShape) {
            m_dragShape = nullptr;
        }
    }

    // Обновляем видимость и значения инспектора
    UpdateInspector();
}

void PlayScene::OnRender(m2d::Renderer& renderer) {
    renderer.Clear(30, 30, 30);

    for (const auto& shape : m_shapes) {
        shape.Render(renderer);
    }
}

// ===== Внутреннее =====

m2d::Shape* PlayScene::FindShapeAt(float x, float y) {
    for (auto it = m_shapes.rbegin(); it != m_shapes.rend(); ++it) {
        if (it->ContainsPoint(x, y)) {
            return &(*it);
        }
    }
    return nullptr;
}

m2d::Shape* PlayScene::FindSelectedShape() {
    for (auto& shape : m_shapes) {
        if (shape.IsSelected()) return &shape;
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
        DeselectAll();
        return;
    }

    if (hit->IsSelected()) {
        hit->SetSelected(false);
        m_dragShape = nullptr;
    } else {
        DeselectAll();
        hit->SetSelected(true);
    }
}

void PlayScene::HandleShapeDrag(float mx, float my) {
    if (m_dragShape) {
        m_dragShape->MoveTo(mx - m_dragOffsetX, my - m_dragOffsetY);
        return;
    }

    m2d::Shape* hit = FindShapeAt(mx, my);
    if (hit && hit->IsSelected()) {
        m_dragShape = hit;
        m_dragOffsetX = mx - hit->GetX();
        m_dragOffsetY = my - hit->GetY();
    }
}

void PlayScene::UpdateInspector() {
    auto* selected = FindSelectedShape();

    if (!selected) {
        // Ничего не выделено — скрыть инспектор
        if (m_inspectorPanel->IsVisible()) {
            m_inspectorPanel->SetVisible(false);
        }
        return;
    }

    // Есть выделенная фигура — показать и обновить
    if (!m_inspectorPanel->IsVisible()) {
        m_inspectorPanel->SetVisible(true);
    }

    // Обновляем тип (на случай, если пользователь выделил другую фигуру)
    std::string typeText = std::string("Type: ") + selected->GetTypeName();
    m_inspectorTypeLabel->SetText(typeText);

    // Обновляем scale
    const float scale = selected->GetScale();
    char buf[64];
    snprintf(buf, sizeof(buf), "Scale: %.2f", scale);
    m_inspectorScaleLabel->SetText(buf);

    // Обновляем слайдер, только если он не тащится сейчас
    if (!m_inspectorScaleSlider->IsDragging()) {
        m_inspectorScaleSlider->SetValue(scale);
    }
}

// ===== Фигуры =====

void PlayScene::AddRandomShape(m2d::Shape::Type type) {
    constexpr float LEFT_W  = 220.0f;
    constexpr float RIGHT_W = 260.0f;
    constexpr float MARGIN  = 60.0f;

    const int W = m_engine->GetWidth();
    const int H = m_engine->GetHeight();

    const float size = m2d::Shape::BASE_SIZE;

    const float minX = LEFT_W + MARGIN;
    const float maxX = (float)W - RIGHT_W - size - MARGIN;
    const float minY = MARGIN;
    const float maxY = (float)H - size - MARGIN - 40.0f;

    if (maxX <= minX || maxY <= minY) {
        M2D_WARN("No space for new shape");
        return;
    }

    const float x = RandomFloat(minX, maxX);
    const float y = RandomFloat(minY, maxY);

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

    m_shapes.emplace_back(type, x, y,
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