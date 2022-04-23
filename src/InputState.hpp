#ifndef SRC_INPUT_STATE_HPP
#define SRC_INPUT_STATE_HPP

#include "State.hpp"

#include "TextButton.hpp"

class InputState : public State
{
public:
    InputState(rl::Window &p_window, std::stack<std::unique_ptr<State>> &p_states, AssetsManager &p_assetsManager, bool p_soundOn);

    void LoadTexturesRects();

    void Update() override;
    void Draw() override;

private:
    Dictionary<TextButton> m_textButtons;
};

#endif // SRC_INPUT_STATE_HPP
