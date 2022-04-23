#ifndef SRC_BLOCK_HPP
#define SRC_BLOCK_HPP

#include "operators.hpp"

class Block
{
public:
    Block(const rl::Rectangle &p_rect, const rl::Vector2 &p_pos=rl::Vector2{ 0.f, 0.f })
        : m_srcRect{ p_rect }, m_pos{ p_pos }
    {
        
    }

    bool IsSelected() const noexcept { return m_color != rl::Color::White(); }
    bool IsHovered() const noexcept { return rl::Mouse::GetPosition().CheckCollision(rl::Rectangle{ m_pos, m_srcRect.GetSize() }); }

    rl::Rectangle GetTextureRect() const noexcept { return m_srcRect; }
    void SetTextureRect(const rl::Rectangle &p_rect) 
    {
        if (m_srcRect != p_rect)
        {
            m_srcRect = p_rect;
        }
    }

    rl::Vector2 GetPosition() const noexcept { return m_pos; }

    void SetPosition(const rl::Vector2 &p_newPos)
    {
        if (m_pos != p_newPos)
        {
            m_pos = p_newPos;
        }
    }

    void SetPosition(float p_x, float p_y)
    {
        this->SetPosition({ p_x, p_y });
    }

    void Move(const rl::Vector2 &p_offset)
    {
        this->SetPosition(m_pos + p_offset);
    }

    void Move(float p_x, float p_y)
    {
        this->Move({ p_x, p_y });
    }

    void SetFillColor(const rl::Color &p_color)
    {
        m_color = p_color;
    }

    void Draw(rl::Texture2D &p_texture)
    {
        p_texture.Draw(m_srcRect, m_pos, m_color);
    }

private:
    rl::Rectangle m_srcRect;
    rl::Vector2 m_pos;
    rl::Color m_color{ rl::Color::White() };
};

#endif // SRC_BLOCK_HPP
