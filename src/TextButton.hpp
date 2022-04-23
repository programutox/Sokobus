#ifndef SRC_TEXT_BUTTON_HPP
#define SRC_TEXT_BUTTON_HPP

#include "raylib-cpp.hpp"

class TextButton
{
public:
    TextButton(rl::Font &p_font, const std::string &p_str, int p_size, const rl::Vector2 &p_pos);

    bool IsHovered() const noexcept { return m_rect.CheckCollision(rl::Mouse::GetPosition()); }
    bool IsClicked() const noexcept { return this->IsHovered() && rl::Mouse::IsButtonPressed(MOUSE_BUTTON_LEFT); }

    rl::Vector2 GetPosition() const noexcept { return m_rect.GetPosition(); }

    void Draw();

private:
    rl::Font &m_font;
    const std::string m_str;
    const int m_size;
    const rl::Rectangle m_rect;
    rl::Color m_color1{ rl::Color::Black() };
    rl::Color m_color2{ rl::Color::White() };
};

#endif // SRC_TEXT_BUTTON_HPP
