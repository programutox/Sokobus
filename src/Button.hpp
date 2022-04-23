#ifndef SRC_BUTTON_HPP
#define SRC_BUTTON_HPP

#include "raylib-cpp.hpp"

enum class InputMode { ZQSD, WASD, MOBILE };

class Button
{
public:
    Button(rl::Texture2D &p_texture, const rl::Rectangle &p_normalSrcRect, const rl::Rectangle &p_hoveredSrcRect, const rl::Vector2 &p_pos);

    rl::Vector2 GetPosition() const noexcept { return m_rect.GetPosition(); }
    void SetPosition(const rl::Vector2 &p_pos) 
    { 
        if (m_rect.GetPosition() != p_pos)
        {
            m_rect.SetPosition(p_pos);
        }
    }
    
    bool IsHovered() const noexcept { return m_rect.CheckCollision(rl::Mouse::GetPosition()); }
    bool IsClicked() const noexcept { return this->IsHovered() && rl::Mouse::IsButtonPressed(MOUSE_LEFT_BUTTON); }
    bool IsDown() const noexcept { return this->IsHovered() && rl::Mouse::IsButtonDown(MOUSE_LEFT_BUTTON); }

    void DrawLine() noexcept;
    void Draw(InputMode p_mode=InputMode::WASD) noexcept;

private:
    rl::Texture2D &m_texture;
    const rl::Rectangle m_normalSrcRect;
    const rl::Rectangle m_hoveredSrcRect;
    rl::Rectangle m_rect;
    rl::Color m_color{ rl::Color::White() };
};

#endif // SRC_BUTTON_HPP
