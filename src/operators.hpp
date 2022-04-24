#ifndef SRC_OPERATORS_HPP
#define SRC_OPERATORS_HPP

#include "raylib-cpp.hpp"

/*
 * Operators for Rectangle and Color comparisons are defined here (outside of their class defintions) because
 * if they are not you get errors from emcc.
 */

inline bool operator==(const rl::Rectangle &p_left, const rl::Rectangle &p_right)
{
    return p_left.x == p_right.x && p_left.y == p_right.y && p_left.width == p_right.width && p_left.height == p_right.height;
}

inline bool operator!=(const rl::Rectangle &p_left, const rl::Rectangle &p_right)
{
    return !(p_left == p_right);
}

inline bool operator==(const rl::Color &p_left, const rl::Color &p_right)
{
    return p_left.r == p_right.r && p_left.g == p_right.g && p_left.b == p_right.b && p_left.a == p_right.a;
}

inline bool operator!=(const rl::Color &p_left, const rl::Color &p_right)
{
    return !(p_left == p_right);
}

#endif // SRC_OPERATORS_HPP
