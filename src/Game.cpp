#include "Game.hpp"

#include <array>
#include <ctime>
#include <fstream>

#include "InputState.hpp"
#include "MenuState.hpp"

Game::Game()
{
    m_window.SetIcon(rl::Image{ "../assets/img/crate.png" });
    SetExitKey(0);

    #if !defined(PLATFORM_WEB)
        m_states.emplace(std::make_unique<MenuState>(m_window, m_states, m_assetsManager, true, InputMode::WASD));
    #else
        m_states.emplace(std::make_unique<InputState>(m_window, m_states, m_assetsManager, true));
    #endif
}

std::string Game::GetCurrentDateTime() noexcept
{
    constexpr const char *format { "%d/%m/%Y at %X" };
    constexpr std::size_t bufferSize { sizeof("dd/mm/yyyy at hh:mm:ss") };

    const std::time_t now{ std::time(nullptr) };
    const std::tm tstruct{ *std::localtime(&now) };

    std::array<char, bufferSize> buffer{ };
    std::strftime(buffer.data(), buffer.size(), format, &tstruct);

    return buffer.data();
}

void Game::LogError(const std::string &p_message)
{
    std::ofstream ofs { "log.txt", std::ios::app };
    if (ofs.is_open())
    {
        ofs << Game::GetCurrentDateTime() << " : " << p_message << '\n';
    }
}

void Game::Update()
{
    m_states.top()->Update();
}

void Game::Draw()
{
    m_window.BeginDrawing();

        m_window.ClearBackground({ 239, 227, 175 });

        m_states.top()->Draw();

    m_window.EndDrawing();
}
