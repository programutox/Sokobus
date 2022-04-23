#ifndef SRC_CLOCK_HPP
#define SRC_CLOCK_HPP

#include "raylib-cpp.hpp"

using Time = double;

class Clock
{
public:
    Clock() : m_time{ GetTime() }
    {

    }

    // Returns elapsed time as seconds
    Time GetElapsedTime() const noexcept { return GetTime() - m_time; }
    
    Time Restart() noexcept
    {
        const Time elapsed{ this->GetElapsedTime() };
        m_time = GetTime();
        return elapsed;
    }

private:
    Time m_time;
};

#endif // SRC_CLOCK_HPP
