#ifndef SRC_CHOOSE_LEVEL_STATE_HPP
#define SRC_CHOOSE_LEVEL_STATE_HPP

#include "State.hpp"

class ChooseLevelState : public State
{
public:
    ChooseLevelState(rl::Window &p_window, std::stack<std::unique_ptr<State>> &p_states, AssetsManager &p_assetsManager, bool p_soundOn, 
        InputMode p_mode);

    // Opens file to check which level were completed or not
    void GetLevelsInfo();
    void PageUp();
    void PageDown();
    void Update() override;
    void Draw() override;

private:
    static constexpr int s_levelsRows{ 5 };
    static constexpr int s_levelsColumns{ 6 };
    static constexpr int s_levelsPerPage{ s_levelsRows * s_levelsColumns };
    static constexpr int s_textSize{ 20 };
    const int m_numberOfLevels{ m_assetsManager.GetNumbersOfLinesOfFile("../assets/data/levels.lvl") };
    // const int m_numberOfLevels{ 70 };
    const int m_numberOfPages{ m_numberOfLevels / s_levelsPerPage + 1 };
    
    int m_currentPage{ 1 };
    bool m_choseLevel{ };

    InputMode m_mode;
    std::vector<int> m_completedLevels;
    std::vector<rl::Rectangle> m_levelsRects;
};

#endif // SRC_CHOOSE_LEVEL_STATE_HPP
