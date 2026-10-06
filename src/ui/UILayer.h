#pragma once

#include <vector>
#include <memory>
#include "ui/UIElement.h"
#include "ui/UIButton.h"
#include "ui/UISlider.h"
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

    bool IsPointOverUI(float x, float y) const;

private:
    void UpdateElement(UIElement* element,
                       float mx, float my,
                       bool mouseDown, bool mouseReleased);

    bool IsPointOverElement(const UIElement* element,
                            float x, float y) const;

    std::vector<std::unique_ptr<UIElement>> m_elements;
    Font* m_font = nullptr;
    bool  m_visible = true;

    bool m_mouseWasDownLastFrame = false;
    UISlider* m_activeSlider = nullptr;   // слайдер, который тащат
};

} // namespace m2d