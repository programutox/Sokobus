#ifndef SRC_GAME_STATE_HPP
#define SRC_GAME_STATE_HPP

#include "State.hpp"

#include <gsl/gsl> // For gsl::index

#include "Clock.hpp"
#include "Group.hpp"
#include "Player.hpp"

class GameState : public State
{
public:
    GameState(rl::Window &p_window, std::stack<std::unique_ptr<State>> &p_states, AssetsManager &p_assetsManager, bool p_soundOn,
        InputMode p_mode, int p_level=1);

    void AddButtons();

    // @returns The number of line of a file (including newlines)
    int GetNumberOfLinesOfFile() const;
    // Writes in a file the numbers of completed levels
    void SaveProgress(bool p_allLevelsCompleted=false) const;

    // Extracts the data from a file line and according to indexes (each digit corresponds to a specific block)
    void HandleFileLine(const std::string &p_line, gsl::index p_x, gsl::index p_y);
    void LoadLevel();

    /* 
        * @param p_group The group you want to check.
        * @param p_pos The position of the block.
        * @param p_id The block's texture rect id. 
        * @returns True if a block from the group at this pos exists, and if it corresponds to the id.
        */
    bool IsSpecificBlock(const Group &p_group, const rl::Vector2 &p_pos, const std::string &p_id) const
    {
        const auto it{ 
            p_group.FindIf([p_pos](const auto &block) { return block.GetPosition().x == p_pos.x && block.GetPosition().y == p_pos.y; })
        };
        return it != p_group.cend() && it->GetTextureRect() == m_assetsManager.GetRect(p_id);
    }

    bool IsSpecificStaticBlock(const rl::Vector2 &p_pos, const std::string &p_id) const
    {
        return this->IsSpecificBlock(m_staticBlocks, p_pos, p_id);
    }

    bool IsSpecificCrate(const rl::Vector2 &p_pos, const std::string &p_id) const
    {
        return this->IsSpecificBlock(m_crateBlocks, p_pos, p_id);
    }

    bool IsWall(const rl::Vector2 &p_pos) const
    {
        return this->IsSpecificStaticBlock(p_pos, "wall");
    }

    bool IsObjective(const rl::Vector2 &p_pos) const
    {
        return this->IsSpecificStaticBlock(p_pos, "objective");
    }

    bool IsCrate(const rl::Vector2 &p_pos) const
    {
        return this->IsSpecificCrate(p_pos, "crate");
    }
    
    bool IsOkCrate(const rl::Vector2 &p_pos) const
    {
        return this->IsSpecificCrate(p_pos, "ok_crate");
    }

    // @returns True if the block exists and is either a crate or an ok_crate.
    bool IsAnyCrate(const rl::Vector2 &p_pos) const
    {
        return this->IsCrate(p_pos) || this->IsOkCrate(p_pos);
    }

    // @returns True if there is a crate with a wall or another crate behind it.
    bool IsCrateBlocked(const rl::Vector2 &p_cratePos, const rl::Vector2 &p_blockBehindPos) const
    {
        return this->IsAnyCrate(p_cratePos) && (this->IsWall(p_blockBehindPos) || this->IsAnyCrate(p_blockBehindPos));
    }
    
    void UpdateDigitsFromTimeText(int p_time, bool p_addColon=true);
    void UpdateTimeText();
    
    void UpdateMovementBooleans();

    // Moves the crate in front of the player if possible
    void MoveCrate(const rl::Vector2 &p_cratePos, const rl::Vector2 &p_crateOffset);
    void UpdateMoves(const rl::Vector2 &p_closestPos, const rl::Vector2 &p_offset, const std::string &p_playerTextureRectId);
    void HandlePlayerMovement();

    void Update() override;
    void Draw() override;

private:
    // The time interval during which the player cannot move when key is down
    static constexpr Time s_moveInterval{ 0.2 };
    bool m_keyRepeat{ };

    InputMode m_mode;
    bool m_paused{ };
    Time m_totalPauseTime{ };

    Player m_player{ m_assetsManager.GetTexture("spritesheet") };

    // Regroups the wall and the objective blocks
    Group m_staticBlocks{ m_assetsManager.GetTexture("spritesheet") };
    // Regroups the crate and ok_crate blocks
    Group m_crateBlocks{ m_assetsManager.GetTexture("spritesheet") };

    const int m_levelsMax{ m_assetsManager.GetNumbersOfLinesOfFile("../assets/data/levels.lvl") };
    bool m_objectivesRemaining{ };
    int m_level;

    int m_numberOfSteps{ };
    std::string m_numberOfStepsStr{ "0" };

    Dictionary<bool> m_moves;

    Dictionary<Clock> m_clocks;
    std::string m_timeStr;
};

#endif // SRC_GAME_STATE_HPP
