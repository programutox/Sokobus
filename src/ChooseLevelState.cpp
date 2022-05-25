#include "ChooseLevelState.hpp"

#include <fstream>

#include "constants.hpp"
#include "GameState.hpp"

ChooseLevelState::ChooseLevelState(rl::Window &p_window, std::stack<std::unique_ptr<State>> &p_states, AssetsManager &p_assetsManager, 
    bool p_soundOn, InputMode p_mode) 
        : State{ p_window, p_states, p_assetsManager, p_soundOn }, m_mode{ p_mode }
{
    m_assetsManager.AddButton("exit", "spritesheet", "back", "back_hovered", { 10.f, 10.f });
    
    if (p_mode == InputMode::MOBILE)
    {
        m_assetsManager.AddButton(
            "scroll_up", "spritesheet", "up", "up_hovered", 
            { m_window.GetSize().x - blockSize - 10.f, m_window.GetSize().y - 2 * (blockSize + 10.f) }
        );
        m_assetsManager.AddButton(
            "scroll_down", "spritesheet", "down", "down_hovered", 
            { m_window.GetSize().x - blockSize - 10.f, m_window.GetSize().y - blockSize - 10.f }
        );
    }

    this->GetLevelsInfo();

    for (gsl::index i{ 0 }; i < m_numberOfLevels; ++i)
    {
        m_levelsRects.emplace_back(
            51 + blockSize * (i % s_levelsRows) * 2, blockSize + blockSize * (i / s_levelsRows) * 2, blockSize, blockSize
        );
    }
}

void ChooseLevelState::GetLevelsInfo()
{
    using namespace std::string_literals;

    std::ifstream ifs{ "../assets/data/current_level.txt" };
    if (!ifs)
    {
        throw "Unable to open current_level.txt"s;
    }

    if (m_assetsManager.IsFileEmpty(ifs))
    {
        return;
    }

    int value{ };
    while (ifs >> value)
    {
        m_completedLevels.emplace_back(value);
    }
}

void ChooseLevelState::ScrollUp()
{
    if (m_currentPage == 1)
    {
        return;
    }

    for (auto &rect : m_levelsRects)
    {
        rect.y += (m_window.GetHeight() - blockSize);
    }

    --m_currentPage;
    this->PlaySound("selected");
}

void ChooseLevelState::ScrollDown()
{
    if (m_currentPage >= m_numberOfPages)
    {
        return;
    }

    for (auto &rect : m_levelsRects)
    {
        rect.y -= (m_window.GetHeight() - blockSize);
    }

    ++m_currentPage;
    this->PlaySound("selected");
}

void ChooseLevelState::Update()
{
    if (m_assetsManager.GetButton("exit").IsClicked()
        #if !defined(PLATFORM_WEB)
            || IsKeyPressed(KEY_ESCAPE)
        #endif
    )
    {
        this->PlaySound("selected");
        m_states.pop();
        return;
    }

    if (m_choseLevel)
    {
        this->GetLevelsInfo();
        m_choseLevel = false;
    }

    if (IsKeyPressed(KEY_UP) || (m_mode == InputMode::MOBILE && m_assetsManager.GetButton("scroll_up").IsClicked()))
    {
        this->ScrollUp();
    }
    else if (IsKeyPressed(KEY_DOWN) || (m_mode == InputMode::MOBILE && m_assetsManager.GetButton("scroll_down").IsClicked()))
    {
        this->ScrollDown();
    }
    
    for (gsl::index i{ 0 }; const auto &rect : m_levelsRects)
    {
        if (rl::Mouse::IsButtonPressed(MOUSE_BUTTON_LEFT) && rect.CheckCollision(rl::Mouse::GetPosition()))
        {
            m_choseLevel = true;
            this->PlaySound("selected");
            m_states.emplace(std::make_unique<GameState>(m_window, m_states, m_assetsManager, m_soundOn, m_mode, i + 1));
            break;
        }
        ++i;
    }
}

void ChooseLevelState::Draw()
{
    for (gsl::index i{ 0 }; auto &rect : m_levelsRects)
    {
        if (rect.CheckCollision(rl::Mouse::GetPosition()))
        {
            rect.Draw(rl::Color::Gray());
        }
        else if (std::any_of(m_completedLevels.cbegin(), m_completedLevels.cend(), [i](int level) { return (i + 1) == level; }))
        {
            rect.Draw(rl::Color::LightGray());
        }
        else
        {
            rect.Draw(rl::Color::Black());
        }

        const std::string &value{ std::to_string(i + 1) };
        
        m_assetsManager.GetFont("main").DrawText(
            value, { 
                rect.x + (rect.width - m_assetsManager.GetFont("main").MeasureText(value, s_textSize, 0).x) / 2, 
                rect.y + (rect.height - s_textSize) / 2
            }, s_textSize, 0
        );

        ++i;
    }

    if (m_mode == InputMode::MOBILE)
    {
        m_assetsManager.GetButton("scroll_up").Draw(m_mode);
        m_assetsManager.GetButton("scroll_down").Draw(m_mode);
    }

    m_assetsManager.GetButton("exit").Draw(m_mode);
}
