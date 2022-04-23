#ifndef SRC_EDITOR_STATE_HPP
#define SRC_EDITOR_STATE_HPP

#include "State.hpp"

#include <gsl/gsl>

#include <array>

#include "Clock.hpp"
#include "constants.hpp"
#include "Group.hpp"

class EditorState : public State
{
public:
    EditorState(rl::Window &p_window, std::stack<std::unique_ptr<State>> &p_states, AssetsManager &p_assetsManager, bool p_soundOn);

    void HandleFileLine(const std::string &p_line, gsl::index p_x, gsl::index p_y);
    void LoadLevel();

    int GetDigit(Group::iterator p_it);
    // Checks if the level can be saved or not
    bool SaveConditionsRespected();
    void SaveLevel();
    
    void ChangeInformationText(const std::string &p_newText);

    void UpdateButtons();
    void UpdateBlockPosition();
    void HandleKeysForBlocksMovement();

    void Update() override;
    void Draw() override;

private:
    static constexpr Time s_drawTextTime{ 0.5 };

    Group m_blocks{ m_assetsManager.GetTexture("spritesheet") };
    std::array<Block, 5> m_blockTypes{{
        { m_assetsManager.GetRect("wall") },
        { m_assetsManager.GetRect("crate") },
        { m_assetsManager.GetRect("objective") },
        { m_assetsManager.GetRect("player_down") },
        { m_assetsManager.GetRect("ok_crate") }
    }};
    rl::Rectangle m_blockTypeRect;

    // Did the user lock the editor ? If true, you cannot place or delete any blocks, and the buttons will be displayed.
    bool m_locked{ };
    const rl::Rectangle m_paclockRect{ 
        m_window.GetWidth() - blockSize * 1.f, m_window.GetHeight() - blockSize * 1.f, blockSize * 1.f, blockSize * 1.f
    };
    const rl::Vector2 m_padlockPos{ m_window.GetWidth() - blockSize * 1.f, m_window.GetHeight() - blockSize * 1.f };

    bool m_drawText{ };
    std::string m_text;
    Clock m_textClock;

    rl::Rectangle m_currentBlockSrcRect{ m_assetsManager.GetRect("wall") };
    rl::Vector2 m_currentBlockPos;
};

#endif // SRC_EDITOR_STATE_HPP
