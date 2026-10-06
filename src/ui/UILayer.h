#pragma once

#include <vector>
#include <memory>
#include "ui/UIElement.h"
#include "graphics/Font.h"

namespace m2d {

class Input;

// Корневой контейнер для UI движка.
// Рисуется поверх игры, обрабатывает клики ДО игры.
class UILayer {
public:
    UILayer() = default;

    void Init(Font* font) { m_font = font; }
    Font* GetFont() const { return m_font; }

    // ===== Добавление элементов =====
    template<typename T, typename... Args>
    T* Add(Args&&... args) {
        auto element = std::make_unique<T>(std::forward<Args>(args)...);
        T* raw = element.get();
        m_elements.push_back(std::move(element));
        return raw;
    }

    // ===== Жизненный цикл =====
    void Update(float dt, const Input& input);
    void Render(Renderer& renderer);

    // ===== Управление =====
    void SetVisible(bool v) { m_visible = v; }
    bool IsVisible() const  { return m_visible; }
    void ToggleVisible()    { m_visible = !m_visible; }

    // Проверка: попадает ли точка в какой-либо UI-элемент.
    // Если да — игра не должна получать этот клик.
    bool IsPointOverUI(float x, float y) const;

private:
    std::vector<std::unique_ptr<UIElement>> m_elements;
    Font* m_font = nullptr;
    bool  m_visible = true;
};

} // namespace m2d