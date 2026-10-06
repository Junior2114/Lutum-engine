#include "my_game/PlayScene.h"
#include "my_game/MyGame.h"
#include "core/Log.h"

#include <random>
#include <algorithm>
#include <string>
#include <cmath>
#include <cstdio>

namespace mygame {

static float RandomFloat(float min, float max) {
    static std::mt19937 gen{ std::random_device{}() };
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}

static constexpr float DRAG_THRESHOLD = 3.0f;

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
        btn->SetOnClick([this, type]() { AddRandomShape(type); });
        y += 46.0f;
    }

    y += 20.0f;
    auto* clearBtn = leftPanel->AddChild<m2d::UIButton>();
    clearBtn->SetPosition(16.0f, y);
    clearBtn->SetSize(LEFT_W - 32.0f, 36.0f);
    clearBtn->SetFont(uiFont);
    clearBtn->SetText("Clear");
    clearBtn->SetColors(120, 40, 40, 160, 60, 60, 90, 30, 30);
    clearBtn->SetOnClick([this]() { ClearShapes(); });

    // ===== Правая панель — Inspector =====
    constexpr float RIGHT_W = 280.0f;

    m_inspectorPanel = ui.Add<m2d::UIPanel>();
    m_inspectorPanel->SetPosition((float)W - RIGHT_W, 0.0f);
    m_inspectorPanel->SetSize(RIGHT_W, (float)H);
    m_inspectorPanel->SetBackgroundColor(22, 22, 28, 245);
    m_inspectorPanel->SetBorder(true, 55, 55, 75);
    m_inspectorPanel->SetVisible(false);

    // Header
    auto* header = m_inspectorPanel->AddChild<m2d::UILabel>();
    header->SetPosition(16.0f, 14.0f);
    header->SetFont(uiFont);
    header->SetText("INSPECTOR");
    header->SetColor(140, 140, 180);

    auto* sep1 = m_inspectorPanel->AddChild<m2d::UISeparator>();
    sep1->SetPosition(16.0f, 44.0f);
    sep1->SetSize(RIGHT_W - 32.0f, 4.0f);
    sep1->SetColor(55, 55, 75);

    // OBJECT
    auto* secObj = m_inspectorPanel->AddChild<m2d::UILabel>();
    secObj->SetPosition(16.0f, 60.0f);
    secObj->SetFont(uiFont);
    secObj->SetText("OBJECT");
    secObj->SetColor(100, 180, 255);

    m_inspectorTypeLabel = m_inspectorPanel->AddChild<m2d::UILabel>();
    m_inspectorTypeLabel->SetPosition(20.0f, 90.0f);
    m_inspectorTypeLabel->SetFont(uiFont);
    m_inspectorTypeLabel->SetText("Type: -");
    m_inspectorTypeLabel->SetColor(220, 220, 220);

    auto* sep2 = m_inspectorPanel->AddChild<m2d::UISeparator>();
    sep2->SetPosition(16.0f, 122.0f);
    sep2->SetSize(RIGHT_W - 32.0f, 4.0f);
    sep2->SetColor(55, 55, 75);

    // TRANSFORM
    auto* secTf = m_inspectorPanel->AddChild<m2d::UILabel>();
    secTf->SetPosition(16.0f, 140.0f);
    secTf->SetFont(uiFont);
    secTf->SetText("TRANSFORM");
    secTf->SetColor(100, 180, 255);

    // ===== Scale X =====
    auto* sxLabel = m_inspectorPanel->AddChild<m2d::UILabel>();
    sxLabel->SetPosition(20.0f, 170.0f);
    sxLabel->SetFont(uiFont);
    sxLabel->SetText("Scale X");
    sxLabel->SetColor(200, 200, 200);

    m_scaleXSlider = m_inspectorPanel->AddChild<m2d::UISlider>();
    m_scaleXSlider->SetPosition(20.0f, 200.0f);
    m_scaleXSlider->SetSize(RIGHT_W - 130.0f, 20.0f);
    m_scaleXSlider->SetRange(0.2f, 3.0f);
    m_scaleXSlider->SetValue(1.0f);
    m_scaleXSlider->SetOnChange([this](float value) {
        auto* shape = FindSelectedShape();
        if (shape) {
            shape->SetScaleX(value);
            char buf[64];
            std::snprintf(buf, sizeof(buf), "%.2f", value);
            m_scaleXValue->SetText(buf);
        }
    });

    m_scaleXValue = m_inspectorPanel->AddChild<m2d::UITextInput>();
    m_scaleXValue->SetPosition(RIGHT_W - 95.0f, 200.0f);
    m_scaleXValue->SetSize(70.0f, 20.0f);
    m_scaleXValue->SetFont(uiFont);
    m_scaleXValue->SetText("1.00");
    m_scaleXValue->SetOnSubmit([this](const std::string& text) {
        auto* shape = FindSelectedShape();
        if (!shape) return;
        try {
            float v = std::stof(text);
            shape->SetScaleX(v);
            char buf[64];
            std::snprintf(buf, sizeof(buf), "%.2f", shape->GetScaleX());
            m_scaleXValue->SetText(buf);
        } catch (...) {
            M2D_WARN("Invalid Scale X input: ", text);
        }
    });

    // ===== Scale Y =====
    auto* syLabel = m_inspectorPanel->AddChild<m2d::UILabel>();
    syLabel->SetPosition(20.0f, 240.0f);
    syLabel->SetFont(uiFont);
    syLabel->SetText("Scale Y");
    syLabel->SetColor(200, 200, 200);

    m_scaleYSlider = m_inspectorPanel->AddChild<m2d::UISlider>();
    m_scaleYSlider->SetPosition(20.0f, 270.0f);
    m_scaleYSlider->SetSize(RIGHT_W - 130.0f, 20.0f);
    m_scaleYSlider->SetRange(0.2f, 3.0f);
    m_scaleYSlider->SetValue(1.0f);
    m_scaleYSlider->SetOnChange([this](float value) {
        auto* shape = FindSelectedShape();
        if (shape) {
            shape->SetScaleY(value);
            char buf[64];
            std::snprintf(buf, sizeof(buf), "%.2f", value);
            m_scaleYValue->SetText(buf);
        }
    });

    m_scaleYValue = m_inspectorPanel->AddChild<m2d::UITextInput>();
    m_scaleYValue->SetPosition(RIGHT_W - 95.0f, 270.0f);
    m_scaleYValue->SetSize(70.0f, 20.0f);
    m_scaleYValue->SetFont(uiFont);
    m_scaleYValue->SetText("1.00");
    m_scaleYValue->SetOnSubmit([this](const std::string& text) {
        auto* shape = FindSelectedShape();
        if (!shape) return;
        try {
            float v = std::stof(text);
            shape->SetScaleY(v);
            char buf[64];
            std::snprintf(buf, sizeof(buf), "%.2f", shape->GetScaleY());
            m_scaleYValue->SetText(buf);
        } catch (...) {
            M2D_WARN("Invalid Scale Y input: ", text);
        }
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

    if (!uiHovered) {
        if (m_input->IsMouseButtonPressed(SDL_BUTTON_LEFT)) {
            OnMousePressed(mx, my);
        }
        if (m_input->IsMouseButtonDown(SDL_BUTTON_LEFT)) {
            OnMouseHeld(mx, my);
        }
    }

    if (m_input->IsMouseButtonReleased(SDL_BUTTON_LEFT)) {
        OnMouseReleased();
    }

    UpdateInspector();
}

void PlayScene::OnRender(m2d::Renderer& renderer) {
    renderer.Clear(30, 30, 30);
    for (const auto& shape : m_shapes) {
        shape.Render(renderer);
    }
}

void PlayScene::OnMousePressed(float mx, float my) {
    m2d::Shape* hit = FindShapeAt(mx, my);
    if (hit) {
        m_pressedShape = hit;
        m_pressStartX = mx;
        m_pressStartY = my;
        m_dragStarted = false;
    } else {
        DeselectAll();
        m_pressedShape = nullptr;
    }
}

void PlayScene::OnMouseHeld(float mx, float my) {
    if (m_dragShape) {
        m_dragShape->MoveBy(mx - m_dragShape->GetCenterX(),
                            my - m_dragShape->GetCenterY());
        return;
    }

    if (m_pressedShape && !m_dragStarted) {
        const float dx = mx - m_pressStartX;
        const float dy = my - m_pressStartY;
        if (std::sqrt(dx * dx + dy * dy) > DRAG_THRESHOLD) {
            DeselectAll();
            m_pressedShape->SetSelected(true);
            m_dragShape = m_pressedShape;
            m_dragOffsetX = m_pressStartX - m_dragShape->GetCenterX();
            m_dragOffsetY = m_pressStartY - m_dragShape->GetCenterY();
            m_dragStarted = true;
        }
    }
}

void PlayScene::OnMouseReleased() {
    if (m_pressedShape && !m_dragStarted) {
        if (m_pressedShape->IsSelected()) {
            m_pressedShape->SetSelected(false);
        } else {
            DeselectAll();
            m_pressedShape->SetSelected(true);
        }
    }
    m_pressedShape = nullptr;
    m_dragShape = nullptr;
    m_dragStarted = false;
}

m2d::Shape* PlayScene::FindShapeAt(float x, float y) {
    for (auto it = m_shapes.rbegin(); it != m_shapes.rend(); ++it) {
        if (it->ContainsPoint(x, y)) return &(*it);
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

void PlayScene::UpdateInspector() {
    auto* selected = FindSelectedShape();

    if (!selected) {
        if (m_inspectorPanel->IsVisible()) {
            m_inspectorPanel->SetVisible(false);
        }
        return;
    }

    if (!m_inspectorPanel->IsVisible()) {
        m_inspectorPanel->SetVisible(true);
    }

    std::string typeText = std::string("Type: ") + selected->GetTypeName();
    m_inspectorTypeLabel->SetText(typeText);

    const float sx = selected->GetScaleX();
    const float sy = selected->GetScaleY();

    if (!m_scaleXSlider->IsDragging()) m_scaleXSlider->SetValue(sx);
    if (!m_scaleYSlider->IsDragging()) m_scaleYSlider->SetValue(sy);

    if (!m_scaleXValue->IsFocused()) {
        char buf[64];
        std::snprintf(buf, sizeof(buf), "%.2f", sx);
        m_scaleXValue->SetText(buf);
    }
    if (!m_scaleYValue->IsFocused()) {
        char buf[64];
        std::snprintf(buf, sizeof(buf), "%.2f", sy);
        m_scaleYValue->SetText(buf);
    }
}

void PlayScene::AddRandomShape(m2d::Shape::Type type) {
    constexpr float LEFT_W  = 220.0f;
    constexpr float RIGHT_W = 280.0f;
    constexpr float MARGIN  = 60.0f;

    const int W = m_engine->GetWidth();
    const int H = m_engine->GetHeight();
    const float half = m2d::Shape::BASE_SIZE * 0.5f;

    const float minCX = LEFT_W + MARGIN + half;
    const float maxCX = (float)W - RIGHT_W - MARGIN - half;
    const float minCY = MARGIN + half;
    const float maxCY = (float)H - MARGIN - 40.0f - half;

    if (maxCX <= minCX || maxCY <= minCY) {
        M2D_WARN("No space for new shape");
        return;
    }

    const float cx = RandomFloat(minCX, maxCX);
    const float cy = RandomFloat(minCY, maxCY);

    static const Uint8 palette[][3] = {
        { 220,  80,  80 }, {  80, 200, 120 }, {  80, 140, 220 },
        { 220, 180,  80 }, { 180,  80, 220 }, {  80, 200, 200 },
    };
    const int paletteSize = sizeof(palette) / sizeof(palette[0]);
    const int idx = m_shapeCounter % paletteSize;
    ++m_shapeCounter;

    m_shapes.emplace_back(type, cx, cy,
                          palette[idx][0], palette[idx][1], palette[idx][2]);
    M2D_INFO("Added shape at center (", (int)cx, ", ", (int)cy, ")");
}

void PlayScene::ClearShapes() {
    m_shapes.clear();
    m_shapeCounter = 0;
    m_dragShape = nullptr;
    m_pressedShape = nullptr;
    m_dragStarted = false;
    M2D_INFO("Cleared all shapes");
}

} // namespace mygame