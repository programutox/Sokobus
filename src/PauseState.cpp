#include "PauseState.hpp"

#include "constants.hpp"

PauseState::PauseState(rl::Window &p_window, std::stack<std::unique_ptr<State>> &p_states, AssetsManager &p_assetsManager, bool p_soundOn,
    InputMode p_mode)
    : State{ p_window, p_states, p_assetsManager, p_soundOn }, m_mode{ p_mode }
    #if !defined(PLATFORM_WEB)
        , m_screenshot{ "../assets/img/pause.png" }
    #endif
{
    m_assetsManager.AddButton(
        "resume", "spritesheet", "play", "play_hovered", { 
            (m_window.GetWidth() - m_assetsManager.GetRect("play").width) / 2.f,
            (m_window.GetHeight() - m_assetsManager.GetRect("play").height) / 2.f
        }
    );
    m_assetsManager.AddButton("back", "spritesheet", "back", "back_hovered", { 10.f, 10.f });
}

void PauseState::Update()
{
    if (m_assetsManager.GetButton("resume").IsClicked()
        #if !defined(PLATFORM_WEB)
            || IsKeyPressed(KEY_SPACE)
        #endif
    )
    {
        this->PlaySound("selected");
        m_states.pop();
    }
    else if (m_assetsManager.GetButton("back").IsClicked()
        #if !defined(PLATFORM_WEB)
            || IsKeyPressed(KEY_ESCAPE)
        #endif
    )
    {
        this->PlaySound("selected");
        m_states.pop();
        m_states.pop();
    }
}

void PauseState::Draw()
{
    #if !defined(PLATFORM_WEB)
        m_screenshot.Draw();
    #endif
    rl::Color::Gray().Alpha(0.75f).DrawRectangle({ 0.f, 0.f, windowWidth * 1.f, windowHeight * 1.f });

    this->DrawText(
        "Paused", 30, { 
            (m_window.GetWidth() - m_assetsManager.GetFont("main").MeasureText("Paused", 30, 0).x) / 2.f, 
            (m_window.GetHeight() - 30) / 4.f 
        }
    );

    m_assetsManager.GetButton("back").Draw(m_mode);
    m_assetsManager.GetButton("resume").Draw(m_mode);
}
