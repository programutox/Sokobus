#include "Button.hpp"

Button::Button(rl::Texture2D &p_texture, const rl::Rectangle &p_normalSrcRect, const rl::Rectangle &p_hoveredSrcRect, 
    const rl::Vector2 &p_pos) 
    : m_texture{ p_texture }, m_normalSrcRect{ p_normalSrcRect }, m_hoveredSrcRect{ p_hoveredSrcRect }, 
        m_rect{ p_pos, m_normalSrcRect.GetSize() }
{
    
}

void Button::DrawLine() noexcept
{
    rl::Color::White().DrawLine(
        { m_rect.x + m_rect.width, m_rect.y },
        { m_rect.x, m_rect.y + m_rect.height }, 
        3
    );
}

void Button::Draw(InputMode p_mode) noexcept
{
    if (p_mode != InputMode::MOBILE)
    {
        m_texture.Draw(!this->IsHovered() ? m_normalSrcRect : m_hoveredSrcRect, m_rect.GetPosition(), m_color);
        return;
    }

    m_texture.Draw(!this->IsDown() ? m_normalSrcRect : m_hoveredSrcRect, m_rect.GetPosition(), m_color);
}
