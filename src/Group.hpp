#ifndef SRC_GROUP_HPP
#define SRC_GROUP_HPP

#include <vector>

#include "Block.hpp"

class Group : public std::vector<Block>
{
public:
    // @param p_texture Must be a spritesheet
    Group(rl::Texture &p_texture) : m_texture{ p_texture }
    {
        
    }

    template<typename Predicate>
    bool NoneOf(const Predicate &p_pred) const
    {
        return std::none_of(this->cbegin(), this->cend(), p_pred);
    }

    template<typename Predicate>
    bool AnyOf(const Predicate &p_pred) const
    {
        return std::any_of(this->cbegin(), this->cend(), p_pred);
    }

    iterator Find(const rl::Vector2 &p_pos)
    {
        return std::find_if(this->begin(), this->end(), [p_pos](const auto &block) { return block.GetPosition() == p_pos; });
    }

    const_iterator Find(const rl::Vector2 &p_pos) const
    {
        return std::find_if(this->cbegin(), this->cend(), [p_pos](const auto &block) { return block.GetPosition() == p_pos; });
    }

    template<typename Predicate>
    iterator FindIf(const Predicate &p_pred)
    {
        return std::find_if(this->begin(), this->end(), p_pred);
    }

    template<typename Predicate>
    const_iterator FindIf(const Predicate &p_pred) const
    {
        return std::find_if(this->cbegin(), this->cend(), p_pred);
    }

    template<typename Predicate>
    void EraseIf(const Predicate &p_pred)
    {
        this->erase(std::remove_if(this->begin(), this->end(), p_pred), this->end());
    }

    int CountIf(const rl::Rectangle &p_rect) const
    {
        return std::count_if(this->cbegin(), this->cend(), [p_rect](const auto &block) { return block.GetTextureRect() == p_rect; });
    }

    void Draw()
    {
        for (auto &block : *this)
        {
            block.Draw(m_texture);
        }
    }

private:
    rl::Texture &m_texture;
};

#endif // SRC_GROUP_HPP
