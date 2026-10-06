#pragma once

#include <vector>
#include <memory>
#include "ui/UIElement.h"
#include "ui/UIButton.h"
#include "graphics/Font.h"

namespace m2d {

class Input;

class UILayer {
public:
    UILayer() = default;

    void Init(Font* font) { m_font = font; }
    Font* GetFont() const { return m_font; }

    template<typename T, typename... Args>
    T* Add(Args&&... args) {
        auto element = std::make_unique<T>(std::forward<Args>(args)...);
        T* raw = element.get();
        m_elements.push_back(std::move(element));
        return raw;
    }

    void Update(float dt, const Input& input);
    void Render(Renderer& renderer);

    void SetVisible(bool v) { m_visible = v; }
    bool IsVisible() const  { return m_visible; }
    void ToggleVisible()    { m_visible = !m_visible; }

    // Проверка: попадает ли точка в UI.
    // Игра использует это, чтобы не реагировать на клик по UI.
    bool IsPointOverUI(float x, float y) const;

private:
    // Рекурсивный обход элементов — для панелей с детьми.
    void UpdateElement(UIElement* element, const Input& input,
                       float mx, float my, bool mouseDown);

    std::vector<std::unique_ptr<UIElement>> m_elements;
    Font* m_font = nullptr;
    bool  m_visible = true;
};

} // namespace m2d