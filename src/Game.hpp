#ifndef SRC_GAME_HPP
#define SRC_GAME_HPP

#include "AssetsManager.hpp"
#include "constants.hpp"
#include "State.hpp"

class Game
{
public:
    static std::string GetCurrentDateTime() noexcept;
    static void LogError(const std::string &p_message);

    Game();

    void Update();
    void Draw();

private:
    rl::Window m_window{ windowWidth, windowHeight, "Sokobus" };
    rl::AudioDevice m_audioDevice;
    
    std::stack<std::unique_ptr<State>> m_states;
    AssetsManager m_assetsManager{ "../assets/img/", "../assets/sfx/", "../assets/mus/", "../assets/fonts/" };
};

#endif // SRC_GAME_HPP
