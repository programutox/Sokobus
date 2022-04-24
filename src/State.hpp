#ifndef SRC_STATE_HPP
#define SRC_STATE_HPP

#include "raylib-cpp.hpp"

#include <memory>
#include <stack>

#include "AssetsManager.hpp"

class State
{
public:
    State(rl::Window &p_window, std::stack<std::unique_ptr<State>> &p_states, AssetsManager &p_assetsManager, bool p_soundOn);
    virtual ~State();

    // Adds a sound if the game is not mute
    void AddSound(const std::string &p_id, float p_volume=1.f);
    // Plays a sound if the game is not mute
    void PlaySound(const std::string &p_id);
    // Stops a sound if the game is not mute
    void StopSound(const std::string &p_id);

    // Adds a music if the game is not mute
    void AddMusic(const std::string &p_id, const std::string &p_extension, float p_volume=1.f);
    // Plays a music if the game is not mute
    void PlayMusic(const std::string &p_id);
    // Updates a music if the game is not mute
    void UpdateMusic(const std::string &p_id);

    // Draws a white text with a black rectangle behind
    void DrawText(const std::string &p_str, int p_size, const Vector2 &p_pos);
    
    virtual void Update() = 0;
    virtual void Draw() = 0;

protected:
    // Size of the stack in which there is the MenuState
    static constexpr int s_menuStatesIndex{ 
        #if !defined(PLATFORM_WEB)
            1
        #else
            2
        #endif
    };

    rl::Window &m_window;
    std::stack<std::unique_ptr<State>> &m_states;
    AssetsManager &m_assetsManager;
    bool m_soundOn;
};

#endif // SRC_STATE_HPP
