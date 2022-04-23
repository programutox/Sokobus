#ifndef SRC_PLAYER_HPP
#define SRC_PLAYER_HPP

#include <map>
#include <string>

#include "operators.hpp"

class Player
{
public:
    Player(rl::Texture2D &p_texture);

    rl::Vector2 GetPosition() const noexcept { return m_pos; }

    void AddTextureRect(const std::string &p_id, const rl::Rectangle &p_texture, bool p_setRect=false);

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

    void SetTextureRect(const std::string &p_id);
    void Draw();

private:
    rl::Texture &m_texture;
    std::map<std::string, rl::Rectangle> m_srcRects;
    rl::Vector2 m_pos;
    rl::Rectangle m_currentRect;
};

#endif // SRC_PLAYER_HPP
