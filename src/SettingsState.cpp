#include "SettingsState.hpp"

#include <fstream>

#include "constants.hpp"

SettingsState::SettingsState(rl::Window &p_window, std::stack<std::unique_ptr<State>> &p_states, AssetsManager &p_assetsManager, bool p_soundOn)
    : State{ p_window, p_states, p_assetsManager, p_soundOn }
{
    m_assetsManager.AddButton("back", "spritesheet", "back", "back_hovered", { 10.f, 10.f });
    m_assetsManager.AddButton("true", "spritesheet", "true", "true_hovered", {
        m_keyRepeatTextPos.x + m_assetsManager.GetFont("main").MeasureText("Key Repeat", 20, 0).x + 10,
        m_keyRepeatTextPos.y - blockSize / 4
    });
    m_assetsManager.AddButton("false", "spritesheet", "false", "false_hovered", m_assetsManager.GetButton("true").GetPosition());
    m_assetsManager.AddButton("delete_data", "spritesheet", "delete", "delete_hovered", { 
            m_assetsManager.GetButton("true").GetPosition().x, m_assetsManager.GetButton("true").GetPosition().y + blockSize + 10 
        }
    );

    this->GetSettingsInfo();
}

void SettingsState::GetSettingsInfo()
{
    std::ifstream ifs{ "../assets/data/settings.txt" };
    ifs >> m_keyRepeatOn;

    if (m_keyRepeatOn)
    {
        m_keyRepeatButtonId = "true";
    }
}

void SettingsState::WriteSettingsInfo()
{
    std::ofstream ofs{ "../assets/data/settings.txt" };
    ofs << m_keyRepeatOn;
}

void SettingsState::Update()
{
    if (m_assetsManager.GetButton("back").IsClicked())
    {
        this->WriteSettingsInfo();
        this->PlaySound("selected");
        m_states.pop();
    }
    else if (m_assetsManager.GetButton(m_keyRepeatButtonId).IsClicked())
    {
        m_keyRepeatOn = !m_keyRepeatOn;
        m_keyRepeatButtonId = m_keyRepeatOn ? "true" : "false";
        this->PlaySound("selected");
    }
    else if (m_assetsManager.GetButton("delete_data").IsClicked())
    {
        std::ofstream ofs{ "../assets/data/current_level.txt" };
        this->PlaySound("selected");
    }
}

void SettingsState::Draw()
{
    m_assetsManager.GetTexture("spritesheet").Draw(m_assetsManager.GetRect("settings_menu"));
    m_assetsManager.GetButton("back").Draw();
    m_assetsManager.GetButton(m_keyRepeatButtonId).Draw();
    m_assetsManager.GetButton("delete_data").Draw();

    this->DrawText("Key Repeat", 20, m_keyRepeatTextPos);
    this->DrawText("Delete data", 20, { m_keyRepeatTextPos.x, m_assetsManager.GetButton("delete_data").GetPosition().y + 30 / 4 });
}
