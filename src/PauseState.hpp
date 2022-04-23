#ifndef SRC_PAUSE_STATE_HPP
#define SRC_PAUSE_STATE_HPP

#include "State.hpp"

class PauseState : public State
{
public:
    PauseState(rl::Window &p_window, std::stack<std::unique_ptr<State>> &p_states, AssetsManager &p_assetsManager, bool p_soundOn,
        InputMode p_mode);

    void Update() override;
    void Draw() override;

private:
    InputMode m_mode;
    #if !defined(PLATFORM_WEB)
        rl::Texture m_screenshot;
    #endif
};

#endif // SRC_PAUSE_STATE_HPP
