#include "InputState.hpp"

#include "MenuState.hpp"

#include <fstream>

InputState::InputState(rl::Window &p_window, std::stack<std::unique_ptr<State>> &p_states, AssetsManager &p_assetsManager, bool p_soundOn)
    : State{ p_window, p_states, p_assetsManager, p_soundOn }
{
    this->LoadTexturesRects();
    m_assetsManager.Add<rl::Texture2D>("spritesheet");
    m_assetsManager.Add<rl::Font>("main", "PolygonParty");

    m_textButtons.emplace(
        "zqsd", TextButton{ 
            m_assetsManager.GetFont("main"), "ZQSD", 30, { 
                (m_window.GetWidth() - m_assetsManager.GetFont("main").MeasureText("ZQSD", 30, 0).x) / 4.f, 
                (m_window.GetHeight() - 30) / 2.f 
            } 
        }
    );

    m_textButtons.emplace(
        "wasd", TextButton{ 
            m_assetsManager.GetFont("main"), "WASD", 30, { 
                (m_window.GetWidth() - m_assetsManager.GetFont("main").MeasureText("WASD", 30, 0).x) * 3 / 4.f, 
                (m_window.GetHeight() - 30) / 2.f 
            }
        }
    );

    m_textButtons.emplace(
        "mobile", TextButton{ 
            m_assetsManager.GetFont("main"), "Mobile", 30, { 
                (m_window.GetWidth() - m_assetsManager.GetFont("main").MeasureText("Mobile", 30, 0).x) / 2.f, 
                (m_window.GetHeight() - 30) / 2.f + 30 * 2
            }
        }
    );
}

void InputState::LoadTexturesRects()
{
    std::ifstream ifs{ "../assets/data/textures_rects.txt" };
    if (ifs)
    {
        std::string id;
        rl::Rectangle rect;
        while (ifs >> id >> rect.x >> rect.y >> rect.width >> rect.height)
        {
            m_assetsManager.Add(id, rect);
        }
    }
}

void InputState::Update()
{
    if (m_textButtons.at("zqsd").IsClicked())
    {
        m_states.emplace(std::make_unique<MenuState>(m_window, m_states, m_assetsManager, m_soundOn, InputMode::ZQSD));
    }
    else if (m_textButtons.at("wasd").IsClicked())
    {
        m_states.emplace(std::make_unique<MenuState>(m_window, m_states, m_assetsManager, m_soundOn, InputMode::WASD));
    }
    else if (m_textButtons.at("mobile").IsClicked())
    {
        m_states.emplace(std::make_unique<MenuState>(m_window, m_states, m_assetsManager, m_soundOn, InputMode::MOBILE));
    }
}

void InputState::Draw()
{
    this->DrawText(
        "Choose your input", 30, { 
            (m_window.GetWidth() - m_assetsManager.GetFont("main").MeasureText("Choose your input", 30, 0).x) / 2.f, 
            (m_window.GetHeight() - 30) / 4.f 
        }
    );


    for (auto &[id, button] : m_textButtons)
    {
        button.Draw();
    }
}
