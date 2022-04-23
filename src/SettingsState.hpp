#ifndef SRC_SETTINGS_STATE_HPP
#define SRC_SETTINGS_STATE_HPP

#include "State.hpp"

class SettingsState : public State
{
public:
    SettingsState(rl::Window &p_window, std::stack<std::unique_ptr<State>> &p_states, AssetsManager &p_assetsManager, bool p_soundOn);

    // Opens the settings file to get the information
    void GetSettingsInfo();
    void WriteSettingsInfo();

    void Update() override;
    void Draw() override;

private:
    // InputMode m_mode;
    bool m_keyRepeatOn{ };
    const rl::Vector2 m_keyRepeatTextPos{ 
        (m_window.GetWidth() - m_assetsManager.GetFont("main").MeasureText("Key Repeat", 20, 0).x) / 2.f,
        (m_window.GetHeight() - 20) / 2.f
    };

    const char *m_keyRepeatButtonId{ "false" };
};

#endif // SRC_SETTINGS_STATE_HPP
