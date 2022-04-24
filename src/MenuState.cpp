#include "MenuState.hpp"

#include <fstream>

#include "ChooseLevelState.hpp"
#include "EditorState.hpp"
#include "SettingsState.hpp"

MenuState::MenuState(rl::Window &p_window, std::stack<std::unique_ptr<State>> &p_states, AssetsManager &p_assetsManager, bool p_soundOn,
    InputMode p_mode) 
    : State{ p_window, p_states, p_assetsManager, p_soundOn }, m_mode{ p_mode }
{
    if (!m_assetsManager.Exists<rl::Rectangle>("play"))
    {
        this->LoadTexturesRects();
    }

    m_assetsManager.Add<rl::Texture2D>("spritesheet");
    m_assetsManager.Add<rl::Font>("main", "PolygonParty");
    this->AddSound("selected");
    this->AddButtons();

    #if !defined(PLATFORM_WEB)
        this->AddMusic("menu", ".mp3", 0.5f);
        this->PlayMusic("menu");
    #endif
}

void MenuState::AddButtons()
{
#if !defined(PLATFORM_WEB)
    m_assetsManager.AddButton(
        "play", "spritesheet", "play", "play_hovered", { 
            (m_window.GetWidth() - blockSize) / 2.f - 30,
            (m_window.GetHeight() - blockSize) / 2.f + 15
        }
    );

    m_assetsManager.AddButton(
        "editor", "spritesheet", "editor", "editor_hovered", { 
            (m_window.GetWidth() - blockSize) / 2.f + 30,
            (m_window.GetHeight() - blockSize) / 2.f + 15
        }
    );
#else
    m_assetsManager.AddButton(
        "play", "spritesheet", "play", "play_hovered", { 
            (m_window.GetWidth() - blockSize) / 2.f,
            (m_window.GetHeight() - blockSize) / 2.f
        }
    );
#endif

    m_assetsManager.AddButton("settings", "spritesheet", "settings", "settings_hovered", { 10.f, m_window.GetHeight() - blockSize - 10.f });
    m_assetsManager.AddButton(
        "sound", "spritesheet", "sound", "sound_hovered", { 
            m_window.GetWidth() - blockSize - 10.f,
            m_window.GetHeight() - blockSize - 10.f
        }
    );
}

void MenuState::LoadTexturesRects()
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

void MenuState::Update()
{
    #if !defined(PLATFORM_WEB)
        this->UpdateMusic("menu");
    #endif

    if (m_assetsManager.GetButton("play").IsClicked()
        #if !defined(PLATFORM_WEB)
            || IsKeyPressed(KEY_SPACE)
        #endif
    )
    {
        this->PlaySound("selected");
        m_states.emplace(std::make_unique<ChooseLevelState>(m_window, m_states, m_assetsManager, m_soundOn, m_mode));
    }
    #if !defined(PLATFORM_WEB)
        else if (m_assetsManager.GetButton("editor").IsClicked())
        {
            this->PlaySound("selected");
            m_states.emplace(std::make_unique<EditorState>(m_window, m_states, m_assetsManager, m_soundOn));
        }
    #endif
    else if (m_assetsManager.GetButton("settings").IsClicked())
    {
        this->PlaySound("selected");
        m_states.emplace(std::make_unique<SettingsState>(m_window, m_states, m_assetsManager, m_soundOn));
    }
    else if (m_assetsManager.GetButton("sound").IsClicked())
    {
        m_soundOn = !m_soundOn;
    }
}

void MenuState::Draw()
{
    m_assetsManager.GetTexture("spritesheet").Draw(m_assetsManager.GetRect("menu"));
    m_assetsManager.GetButton("play").Draw(m_mode);

    #if !defined(PLATFORM_WEB)
        m_assetsManager.GetButton("editor").Draw(m_mode);
    #endif

    m_assetsManager.GetButton("settings").Draw(m_mode);
    m_assetsManager.GetButton("sound").Draw(m_mode);

    if (!m_soundOn)
    {
        m_assetsManager.GetButton("sound").DrawLine();
    }
}
