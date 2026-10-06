#pragma once

#include <vector>
#include <memory>
#include "core/Scene.h"
#include "core/Engine.h"
#include "ui/UIPanel.h"
#include "ui/UILabel.h"
#include "ui/UIButton.h"
#include "graphics/Font.h"
#include "game/Shape.h"

namespace mygame {

class MyGame;

class PlayScene : public m2d::Scene {
public:
    explicit PlayScene(MyGame* game) : m_game(game) {}

    void OnEnter(m2d::Engine& engine) override;
    void OnExit() override;
    void OnUpdate(float dt) override;
    void OnRender(m2d::Renderer& renderer) override;

private:
    void AddRandomShape(m2d::Shape::Type type);
    void ClearShapes();

    // Работа с фигурами
    m2d::Shape* FindShapeAt(float x, float y);
    void DeselectAll();
    void HandleShapeClick(float mx, float my);
    void HandleShapeDrag(float mx, float my);

    MyGame* m_game = nullptr;
    m2d::Engine* m_engine = nullptr;
    m2d::Input*  m_input  = nullptr;

    m2d::Font* m_font = nullptr;

    std::vector<m2d::Shape> m_shapes;
    int m_shapeCounter = 0;

    // ===== Drag-состояние =====
    m2d::Shape* m_dragShape = nullptr;   // фигура, которую тащим
    float m_dragOffsetX = 0.0f;          // смещение между курсором и фигурой
    float m_dragOffsetY = 0.0f;

    // Для кликов — отслеживаем "был ли клик в этом кадре"
    bool m_mouseWasDown = false;
};

} // namespace mygame