#include "TextButton.hpp"

TextButton::TextButton(rl::Font &p_font, const std::string &p_str, int p_size, const rl::Vector2 &p_pos)
    : m_font{ p_font }, m_str{ p_str }, m_size{ p_size }, 
        m_rect{ p_pos.x - 1, p_pos.y - 1, p_font.MeasureText(p_str, p_size, 0).x + 2, p_size + 1.f }
{
    
}

void TextButton::Draw()
{
    if (!this->IsHovered())
    {
        m_color1.DrawRectangle(m_rect);
        m_font.DrawText(m_str, { m_rect.x + 1, m_rect.y + 1 }, m_size, 0, m_color2);
        return;
    }
    
    m_color2.DrawRectangle(m_rect);
    m_font.DrawText(m_str, { m_rect.x + 1, m_rect.y + 1 }, m_size, 0, m_color1);
}
