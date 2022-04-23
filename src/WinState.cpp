#include "WinState.hpp"

WinState::WinState(rl::Window &p_window, std::stack<std::unique_ptr<State>> &p_states, AssetsManager &p_assetsManager, bool p_soundOn)
    : State{ p_window, p_states, p_assetsManager, p_soundOn }
{
    this->AddSound("win");
    this->PlaySound("win");
}

void WinState::Update()
{
    if (rl::Mouse::IsButtonPressed(MOUSE_BUTTON_LEFT))
    {
        this->StopSound("win");
        
        // Go back to the menu state
        while (m_states.size() > s_menuStatesIndex)
        {
            m_states.pop();
        }
    }
}

void WinState::Draw()
{
    m_assetsManager.GetTexture("spritesheet").Draw(
        m_assetsManager.GetRect(
            #if !defined(PLATFORM_WEB)
                "win_desktop"
            #else
                "win_web"
            #endif
        )
    );
}