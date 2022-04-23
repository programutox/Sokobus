#include "Player.hpp"

Player::Player(rl::Texture2D &p_texture) : m_texture{ p_texture }
{

}

void Player::AddTextureRect(const std::string &p_id, const rl::Rectangle &p_texture, bool p_setRect)
{
    m_srcRects.emplace(p_id, p_texture);
    if (p_setRect)
    {
        m_currentRect = m_srcRects.at(p_id);
    }
}

void Player::SetTextureRect(const std::string &p_id)
{
    if (m_currentRect != m_srcRects.at(p_id))
    {
        m_currentRect = m_srcRects.at(p_id);
    }
}

void Player::Draw()
{
    m_texture.Draw(m_currentRect, m_pos);
}
