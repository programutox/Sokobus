#ifndef SRC_WIN_STATE_HPP
#define SRC_WIN_STATE_HPP

#include "State.hpp"

class WinState : public State
{
public:
    WinState(rl::Window &p_window, std::stack<std::unique_ptr<State>> &p_states, AssetsManager &p_assetsManager, bool p_soundOn);
    
    void Update() override;
    void Draw() override;
};

#endif // SRC_WIN_STATE_HPP
