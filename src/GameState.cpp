#include "GameState.hpp"

#include <fstream>

#include "constants.hpp"
#include "PauseState.hpp"
#include "WinState.hpp"

using namespace std::string_literals;

GameState::GameState(rl::Window &p_window, std::stack<std::unique_ptr<State>> &p_states, AssetsManager &p_assetsManager, bool p_soundOn,
    InputMode p_mode, int p_level)
    : State{ p_window, p_states, p_assetsManager, p_soundOn }, m_mode{ p_mode }, m_level{ p_level }
{
    this->AddSound("walk");
    this->AddSound("finished");

    m_player.AddTextureRect("down", m_assetsManager.GetRect("player_down"), true);
    m_player.AddTextureRect("up", m_assetsManager.GetRect("player_up"));
    m_player.AddTextureRect("left", m_assetsManager.GetRect("player_left"));
    m_player.AddTextureRect("right", m_assetsManager.GetRect("player_right"));

    m_moves.emplace("up", false);
    m_moves.emplace("down", false);
    m_moves.emplace("left", false);
    m_moves.emplace("right", false);
    
    // Gets the time elapsed during a level
    m_clocks.emplace("level", Clock{ });

    // Gets the time elapsed during the PauseState
    m_clocks.emplace("pause", Clock{ });
    m_clocks.emplace("move", Clock{ });

    this->AddButtons();

    this->LoadLevel();

    std::ifstream ifs{ "../assets/data/settings.txt" };
    ifs >> m_keyRepeat;
}

void GameState::AddButtons()
{
    m_assetsManager.AddButton(
        "reset", "spritesheet", "reset", "reset_hovered", { m_window.GetWidth() - m_assetsManager.GetRect("reset").width - 10.f, 10.f }
    );

    m_assetsManager.AddButton("pause", "spritesheet", "pause", "pause_hovered", { 10.f, 10.f });
    
    m_assetsManager.AddButton(
        "up", "spritesheet", "up", "up_hovered", { m_window.GetWidth() - blockSize * 4.f, m_window.GetHeight() - blockSize * 1.f }
    );

    m_assetsManager.AddButton(
        "down", "spritesheet", "down", "down_hovered", { m_window.GetWidth() - blockSize * 3.f, m_window.GetHeight() - blockSize * 1.f }
    );

    m_assetsManager.AddButton(
        "left", "spritesheet", "left", "left_hovered", { m_window.GetWidth() - blockSize * 2.f, m_window.GetHeight() - blockSize * 1.f }
    );

    m_assetsManager.AddButton(
        "right", "spritesheet", "play", "play_hovered", { 
            m_window.GetWidth() - blockSize * 1.f,
            m_window.GetHeight() - blockSize * 1.f,
        }
    );
}

void GameState::SaveProgress(bool p_allLevelsCompleted) const
{
    std::ofstream ofs{ "../assets/data/current_level.txt", std::ios::app };
    if (!ofs)
    {
        throw "Cannot find current_level.txt"s;
    }

    if (!m_assetsManager.IsFileEmpty("../assets/data/current_level.txt"))
    {
        ofs << ' ';
    }

    std::ifstream ifs{ "../assets/data/current_level.txt" };
    std::string line;
    std::getline(ifs, line);

    if (p_allLevelsCompleted)
    {
        if (line.contains(std::to_string(m_levelsMax)))
        {
            return;
        }

        ofs << m_levelsMax;
        return;
    }
    
    if (line.contains(std::to_string(m_level - 1)))
    {
        return;
    }
    // We substract 1 because the current level may be not completed yet
    ofs << (m_level - 1);
}

void GameState::HandleFileLine(const std::string &p_line, gsl::index p_x, gsl::index p_y)
{
    const rl::Vector2 blockPos(p_x * blockSize, p_y * blockSize);
    switch (p_line[(p_y * nbBlocksWidth) + p_x])
    {
        case '0':
            break;

        case '1':
            m_staticBlocks.emplace_back(m_assetsManager.GetRect("wall"), blockPos);
            break;
        
        case '2':
            m_crateBlocks.emplace_back(m_assetsManager.GetRect("crate"), blockPos);
            break;
        
        case '3':
            m_staticBlocks.emplace_back(m_assetsManager.GetRect("objective"), blockPos);
            if (!m_objectivesRemaining)
            {
                m_objectivesRemaining = true;
            }
            break;
        
        case '4':
            m_player.SetTextureRect("down");
            m_player.SetPosition(blockPos);
            break;
        
        case '5':
            m_staticBlocks.emplace_back(m_assetsManager.GetRect("objective"), blockPos);
            m_crateBlocks.emplace_back(m_assetsManager.GetRect("ok_crate"), blockPos);
            break;
        
        default:
            throw "Wrong value given in file levels.lvl"s;
    }
}

void GameState::LoadLevel()
{
    std::ifstream ifs{ "../assets/data/levels.lvl" };
    if (!ifs)
    {
        throw "Cannot find levels.lvl"s;
    }

    if (!m_staticBlocks.empty())
    {
        m_staticBlocks.clear();
        m_crateBlocks.clear();
    }

    // Get first level
    std::string line;
    for (gsl::index i{ 0 }; i < m_level; ++i)
    {
        std::getline(ifs, line);
    }

    for (gsl::index x{ 0 }; x < nbBlocksWidth; ++x)
    {
        for (gsl::index y{ 0 }; y < nbBlocksHeight; ++y)
        {
            this->HandleFileLine(line, x, y);
        }
    }

    // These instructions allow to fill the last whole row with wall blocks
    // because in the editor it is used to choose your item, so you cannot fill it
    const int yPos{ nbBlocksHeight * blockSize };
    for (gsl::index x{ 0 }; x < nbBlocksWidth; ++x)
    {
        const rl::Vector2 blockPos(x * blockSize, yPos);
        m_staticBlocks.emplace_back(m_assetsManager.GetRect("wall"), blockPos);
    }
}

void GameState::UpdateDigitsFromTimeText(int p_time, bool p_addColon)
{
    if (p_time < 10)
    {
        m_timeStr += '0';
    }
    m_timeStr += std::to_string(p_time);
    if (p_addColon)
    {
        m_timeStr += ':';
    }
}

void GameState::UpdateTimeText()
{
    const int elapsedTime{ static_cast<int>(m_clocks.at("level").GetElapsedTime() - m_totalPauseTime) };
    const int hours{ elapsedTime / 3600 };
    const int minutes{ elapsedTime % 3600 / 60 };
    const int seconds{ elapsedTime % 60 };
    
    m_timeStr = "";
    this->UpdateDigitsFromTimeText(hours);
    this->UpdateDigitsFromTimeText(minutes);
    this->UpdateDigitsFromTimeText(seconds, false);
}

void GameState::UpdateMovementBooleans()
{
    switch (m_mode)
    {
        case InputMode::ZQSD:
            m_moves.at("up") = m_keyRepeat ? IsKeyDown(KEY_Z) : IsKeyPressed(KEY_Z);
            m_moves.at("down") = m_keyRepeat ? IsKeyDown(KEY_S) : IsKeyPressed(KEY_S);
            m_moves.at("left") = m_keyRepeat ? IsKeyDown(KEY_Q) : IsKeyPressed(KEY_Q);
            m_moves.at("right") = m_keyRepeat ? IsKeyDown(KEY_D) : IsKeyPressed(KEY_D);
            break;
        
        case InputMode::WASD:
            m_moves.at("up") = m_keyRepeat ? IsKeyDown(KEY_W) : IsKeyPressed(KEY_W);
            m_moves.at("down") = m_keyRepeat ? IsKeyDown(KEY_S) : IsKeyPressed(KEY_S);
            m_moves.at("left") = m_keyRepeat ? IsKeyDown(KEY_A) : IsKeyPressed(KEY_A);
            m_moves.at("right") = m_keyRepeat ? IsKeyDown(KEY_D) : IsKeyPressed(KEY_D);
            break;
        
        case InputMode::MOBILE:
            m_moves.at("up") = m_keyRepeat ? m_assetsManager.GetButton("up").IsDown() : m_assetsManager.GetButton("up").IsClicked();
            m_moves.at("down") = m_keyRepeat ? m_assetsManager.GetButton("down").IsDown() : m_assetsManager.GetButton("down").IsClicked();
            m_moves.at("left") = m_keyRepeat ? m_assetsManager.GetButton("left").IsDown() : m_assetsManager.GetButton("left").IsClicked();
            m_moves.at("right") = m_keyRepeat ? m_assetsManager.GetButton("right").IsDown() : m_assetsManager.GetButton("right").IsClicked();
            break;
        
        default:
            throw "Invalid value for InputMode"s;
    };
}

void GameState::MoveCrate(const rl::Vector2 &p_cratePos, const rl::Vector2 &p_crateOffset)
{
    if (this->IsCrate(p_cratePos))
    {
        auto it{ m_crateBlocks.Find(p_cratePos) };
        it->Move(p_crateOffset);
        if (!this->IsOkCrate(it->GetPosition()) && this->IsObjective(it->GetPosition()))
        {
            it->SetTextureRect(m_assetsManager.GetRect("ok_crate"));
        }
    }
    else if (this->IsOkCrate(p_cratePos))
    {
        auto it{ m_crateBlocks.Find(p_cratePos) };
        it->Move(p_crateOffset);
        if (!this->IsObjective(it->GetPosition()))
        {
            it->SetTextureRect(m_assetsManager.GetRect("crate"));
        }
    }
}

void GameState::UpdateMoves(const rl::Vector2 &p_closestPos, const rl::Vector2 &p_offset, const std::string &p_playerTextureRectId)
{
    this->MoveCrate(p_closestPos, p_offset);
    m_player.Move(p_offset);
    m_player.SetTextureRect(p_playerTextureRectId);
    ++m_numberOfSteps;
    m_numberOfStepsStr = std::to_string(m_numberOfSteps);
    this->PlaySound("walk");
    if (m_keyRepeat)
    {
        m_clocks.at("move").Restart();
    }
}

void GameState::HandlePlayerMovement()
{
    if (m_keyRepeat && m_clocks.at("move").GetElapsedTime() < s_moveInterval)
    {
        return;
    }

    const rl::Vector2 playerPos{ m_player.GetPosition() };
    if (m_moves.at("left"))
    {
        if (playerPos.x <= 0)
        {
            return;
        }
        
        // Position of the closest box to the player (according to its direction)
        const rl::Vector2 closestPos{ playerPos.x - blockSize, playerPos.y };
        if (this->IsWall(closestPos) || this->IsCrateBlocked(closestPos, { playerPos.x - blockSize * 2, playerPos.y }))
        {
            m_player.SetTextureRect("left");
            return;
        }

        this->UpdateMoves(closestPos, { -blockSize * 1.f, 0.f }, "left");
    }
    else if (m_moves.at("right"))
    {
        if (playerPos.x >= blockSize * (nbBlocksWidth - 1))
        {
            return;
        }
        
        const rl::Vector2 closestPos{ playerPos.x + blockSize, playerPos.y };
        if (this->IsWall(closestPos) || this->IsCrateBlocked(closestPos, { playerPos.x + blockSize * 2, playerPos.y }))
        {
            m_player.SetTextureRect("right");
            return;
        }

        this->UpdateMoves(closestPos, { blockSize * 1.f, 0.f }, "right");
    }
    else if (m_moves.at("up"))
    {
        if (playerPos.y <= 0)
        {
            return;
        }

        const rl::Vector2 closestPos{ playerPos.x, playerPos.y - blockSize };
        if (this->IsWall({ playerPos.x, playerPos.y - blockSize }) || this->IsCrateBlocked(closestPos, { playerPos.x, playerPos.y - blockSize * 2 }))
        {
            m_player.SetTextureRect("up");
            return;
        }

        this->UpdateMoves(closestPos, { 0.f, -blockSize * 1.f }, "up");
    }
    else if (m_moves.at("down"))
    {
        if (playerPos.y >= blockSize * nbBlocksHeight)
        {
            return;
        }

        const rl::Vector2 closestPos{ playerPos.x, playerPos.y + blockSize };
        if (this->IsWall(closestPos) || this->IsCrateBlocked(closestPos, { playerPos.x, playerPos.y + blockSize * 2 }))
        {
            m_player.SetTextureRect("down");
            return;
        }

        this->UpdateMoves(closestPos, { 0.f, blockSize * 1.f }, "down");
    }
}

void GameState::Update()
{
    m_objectivesRemaining = m_staticBlocks.CountIf(m_assetsManager.GetRect("objective")) != m_crateBlocks.CountIf(m_assetsManager.GetRect("ok_crate"));
    
    if (!m_objectivesRemaining)
    {
        if (m_level == m_levelsMax)
        {
            this->SaveProgress(true);
            m_states.emplace(std::make_unique<WinState>(m_window, m_states, m_assetsManager, m_soundOn));
            return;
        }

        ++m_level;
        this->LoadLevel();
        this->SaveProgress();
        m_numberOfSteps = 0;
        m_numberOfStepsStr = "0";
        m_totalPauseTime = 0.0;
        m_clocks.at("level").Restart();
        this->PlaySound("finished");
    }

    if (m_paused)
    {
        m_totalPauseTime += m_clocks.at("pause").GetElapsedTime();
        m_paused = false;
    }

    this->UpdateTimeText();

    if (m_assetsManager.GetButton("pause").IsClicked()
        #if !defined(PLATFORM_WEB)
            || IsKeyPressed(KEY_ESCAPE)
        #endif
    )
    {
        m_paused = true;
        m_clocks.at("pause").Restart();
        #if !defined(PLATFORM_WEB)
            rl::TakeScreenshot("../assets/img/pause.png");
        #endif
        this->PlaySound("selected");
        m_states.emplace(std::make_unique<PauseState>(m_window, m_states, m_assetsManager, m_soundOn, m_mode));
        return;
    }
    else if (m_assetsManager.GetButton("reset").IsClicked()
        #if !defined(PLATFORM_WEB)
            || IsKeyPressed('R')
        #endif
    )
    {
        this->LoadLevel();
        m_numberOfSteps = 0;
        m_numberOfStepsStr = "0";
        m_clocks.at("level").Restart();
        this->PlaySound("selected");
    }

    this->UpdateMovementBooleans();
    this->HandlePlayerMovement();
}

void GameState::Draw()
{
    m_staticBlocks.Draw();
    m_crateBlocks.Draw();

    m_player.Draw();

    m_assetsManager.GetButton("reset").Draw(m_mode);
    m_assetsManager.GetButton("pause").Draw(m_mode);

    if (m_mode == InputMode::MOBILE)
    {
        m_assetsManager.GetButton("up").Draw(m_mode);
        m_assetsManager.GetButton("down").Draw(m_mode);
        m_assetsManager.GetButton("left").Draw(m_mode);
        m_assetsManager.GetButton("right").Draw(m_mode);
    }

    this->DrawText(m_numberOfStepsStr, 20, { 0.f, m_window.GetHeight() - 20 * 2.f });
    this->DrawText(m_timeStr, 20, { 0.f, m_window.GetHeight() - 20 * 1.f });
}
