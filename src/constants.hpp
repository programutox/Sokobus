#ifndef SRC_CONSTANTS_HPP
#define SRC_CONSTANTS_HPP

inline constexpr int blockSize{ 34 };
inline constexpr int nbBlocksWidth{ 12 };
inline constexpr int nbBlocksHeight{ 12 };
inline constexpr int windowWidth{ blockSize * nbBlocksWidth };
inline constexpr int windowHeight{ blockSize * (nbBlocksHeight+1) };

enum class Direction { UP, DOWN, LEFT, RIGHT };

#endif // SRC_CONSTANTS_HPP
