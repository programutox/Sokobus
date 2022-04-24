#include "State.hpp"

State::State(rl::Window &p_window, std::stack<std::unique_ptr<State>> &p_states, AssetsManager &p_assetsManager, bool p_soundOn)
    : m_window{ p_window }, m_states{ p_states }, m_assetsManager{ p_assetsManager }, m_soundOn{ p_soundOn }
{
    
}

State::~State()
{

}

void State::AddSound(const std::string &p_id, float p_volume)
{
    if (m_soundOn)
    {
        m_assetsManager.Add<rl::Sound>(p_id);
        m_assetsManager.GetSound(p_id).SetVolume(p_volume);
    }
}

void State::PlaySound(const std::string &p_id)
{
    if (m_soundOn)
    {
        m_assetsManager.GetSound(p_id).Play();
    }
}

void State::StopSound(const std::string &p_id)
{
    if (m_soundOn)
    {
        m_assetsManager.GetSound(p_id).Stop();
    }
}

void State::AddMusic(const std::string &p_id, const std::string &p_extension, float p_volume)
{
    if (m_soundOn)
    {
        m_assetsManager.Add<rl::Music>(p_id, p_extension);
        m_assetsManager.GetMusic(p_id).SetVolume(p_volume);
    }
}

void State::PlayMusic(const std::string &p_id)
{
    if (m_soundOn)
    {
        m_assetsManager.GetMusic(p_id).Play();
    }
}

void State::UpdateMusic(const std::string &p_id)
{
    if (m_soundOn)
    {
        m_assetsManager.GetMusic(p_id).Update();
    }
}

void State::DrawText(const std::string &p_str, int p_size, const Vector2 &p_pos)
{
    rl::Color::Black().DrawRectangle(
        rl::Rectangle{ p_pos.x - 1.f, p_pos.y - 1, m_assetsManager.GetFont("main").MeasureText(p_str, p_size, 0).x + 2.f, p_size + 1.f }
    );
    m_assetsManager.GetFont("main").DrawText(p_str, p_pos, p_size, 0);
}
