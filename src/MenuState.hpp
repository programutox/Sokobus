#ifndef SRC_MENU_STATE_HPP
#define SRC_MENU_STATE_HPP

#include "State.hpp"

class MenuState : public State
{
public:
    MenuState(rl::Window &p_window, std::stack<std::unique_ptr<State>> &p_states, AssetsManager &p_assetsManager, bool p_soundOn,
        InputMode p_mode);

    void AddButtons();

    void LoadTexturesRects();
    
    void Update() override;
    void Draw() override;

private:
    InputMode m_mode;
};

#endif // SRC_MENU_STATE_HPP
