#include "EditorState.hpp"

#include <fstream>

#include "operators.hpp"

using namespace std::string_literals;

EditorState::EditorState(rl::Window &p_window, std::stack<std::unique_ptr<State>> &p_states, AssetsManager &p_assetsManager, bool p_soundOn)
    : State{ p_window, p_states, p_assetsManager, p_soundOn }
{
    m_assetsManager.Add<rl::Sound>("no");
    m_assetsManager.Add<rl::Sound>("finished");
    m_assetsManager.AddButton("reset", "spritesheet", "reset", "reset_hovered", { m_window.GetWidth() - blockSize - 10.f, 10.f });
    m_assetsManager.AddButton("delete", "spritesheet", "delete", "delete_hovered", { m_window.GetWidth() - blockSize * 3 - 10.f, 10.f });
    m_assetsManager.AddButton("save", "spritesheet", "save", "save_hovered", { m_window.GetWidth() - blockSize * 2 - 10.f, 10.f });
    m_assetsManager.AddButton("back", "spritesheet", "back", "back_hovered", { 10.f, 10.f });

    for (gsl::index i{ 0 }; auto &blockType : m_blockTypes)
    {
        blockType.SetPosition(blockSize * (i + 0.125f), m_window.GetHeight() - blockSize * (1 - 0.125f));
        ++i;
    }

    m_blockTypeRect.SetPosition(m_blockTypes[0].GetPosition().x - 2, m_blockTypes[0].GetPosition().y - 2);
    m_blockTypeRect.SetSize(m_blockTypes[0].GetTextureRect().width * 0.75f + 4, m_blockTypes[0].GetTextureRect().height * 0.75f + 4);
    this->LoadLevel();
}

void EditorState::HandleFileLine(const std::string &p_line, gsl::index p_x, gsl::index p_y)
{
    const rl::Vector2 blockPos(p_x * blockSize, p_y * blockSize);
    switch (p_line[(p_y * nbBlocksWidth) + p_x])
    {
        case '0':
            break;

        case '1':
            m_blocks.emplace_back(m_assetsManager.GetRect("wall"), blockPos);
            break;
        
        case '2':
            m_blocks.emplace_back(m_assetsManager.GetRect("crate"), blockPos);
            break;
        
        case '3':
            m_blocks.emplace_back(m_assetsManager.GetRect("objective"), blockPos);
            break;
        
        case '4':
            m_blocks.emplace_back(m_assetsManager.GetRect("player_down"), blockPos);
            break;
        
        case '5':
            m_blocks.emplace_back(m_assetsManager.GetRect("ok_crate"), blockPos);
            break;
        
        default:
            throw "Wrong value given in file levels.lvl"s;
    }
}

void EditorState::LoadLevel()
{
    std::ifstream ifs{ "../assets/data/levels.lvl" };
    if (!ifs)
    {
        throw "Cannot find levels.lvl"s;
    }

    if (!m_blocks.empty())
    {
        m_blocks.clear();
    }

    // Get first level
    std::string line;
    std::getline(ifs, line);

    for (gsl::index x{ 0 }; x < nbBlocksWidth; ++x)
    {
        for (gsl::index y{ 0 }; y < nbBlocksHeight; ++y)
        {
            this->HandleFileLine(line, x, y);
        }
    }
}

int EditorState::GetDigit(Group::iterator p_it)
{
    if (p_it == m_blocks.end())
    {
        return 0;
    }
    
    for (gsl::index i{ 1 }; const auto &blockType : m_blockTypes)
    {
        if (p_it->GetTextureRect() == blockType.GetTextureRect())
        {
            return i;
        }
        ++i;
    }

    throw "Unvalid digit"s;
}

void EditorState::SaveLevel()
{
    std::ofstream ofs{ "../assets/data/levels.lvl", std::ios::app };
    if (!ofs)
    {
        throw "Cannot find levels.lvl"s;
    }

    ofs << '\n';
    for (gsl::index y{ 0 }; y < nbBlocksWidth; ++y)
    {
        for (gsl::index x{ 0 }; x < nbBlocksHeight; ++x)
        {
            ofs << this->GetDigit(m_blocks.Find(rl::Vector2(x, y) * blockSize));
        }
    }
}

bool EditorState::SaveConditionsRespected()
{
    if (m_blocks.empty())
    {
        this->ChangeInformationText("Level empty");
        return false;
    }

    if (m_blocks.NoneOf([this](const auto &block){ return block.GetTextureRect() == m_assetsManager.GetRect("player_down"); }))
    {
        this->ChangeInformationText("No player");
        return false;
    }

    const int numberOfCrates{ m_blocks.CountIf(m_assetsManager.GetRect("crate")) };
    if (numberOfCrates == 0)
    {
        this->ChangeInformationText("No crates");
        return false;
    }

    const int numberOfObjectives{ m_blocks.CountIf(m_assetsManager.GetRect("objective")) };
    if (numberOfObjectives == 0)
    {
        this->ChangeInformationText("No objectives");
        return false;
    }

    if (numberOfCrates > numberOfObjectives)
    {
        this->ChangeInformationText("Too much crates");
        return false;
    }
    
    if (numberOfCrates < numberOfObjectives)
    {
        this->ChangeInformationText("Too much objectives");
        return false;
    }

    return true;
}

void EditorState::ChangeInformationText(const std::string &p_newText)
{
    m_text = p_newText;
    m_drawText = true;
    m_textClock.Restart();
}

void EditorState::UpdateButtons()
{
    if (m_assetsManager.GetButton("back").IsClicked()
        #if !defined(PLATFORM_WEB)
            || IsKeyPressed(KEY_ESCAPE)
        #endif
    )
    {
        this->PlaySound("selected");
        m_states.pop();
    }
    else if (m_assetsManager.GetButton("reset").IsClicked())
    {
        this->ChangeInformationText("Level reloaded");
        this->LoadLevel();
        this->PlaySound("selected");
    }
    else if (m_assetsManager.GetButton("save").IsClicked())
    {
        this->ChangeInformationText("Saved!");
        if (this->SaveConditionsRespected())
        {
            this->SaveLevel();
            this->PlaySound("finished");
            return;
        }

        this->PlaySound("no");
    }
    else if (m_assetsManager.GetButton("delete").IsClicked())
    {
        this->ChangeInformationText("Level deleted");
        m_blocks.clear();
        this->PlaySound("selected");
    }
}

void EditorState::UpdateBlockPosition()
{
    const rl::Vector2 pos{ rl::Mouse::GetPosition() };
    for (gsl::index x{ 0 }; x < nbBlocksWidth; ++x)
    {
        for (gsl::index y{ 0 }; y < nbBlocksHeight; ++y)
        {
            if (pos.x > x * blockSize && pos.x < (x+1) * blockSize && pos.y > y * blockSize && pos.y < (y+1) * blockSize)
            {
                m_currentBlockPos = rl::Vector2(x, y) * blockSize;
                return;
            }
        }
    }
}

void EditorState::HandleKeysForBlocksMovement()
{
    // Left
    if (IsKeyPressed('A'))
    {
        if (!m_blocks.empty() && m_blocks.NoneOf([](const Block &block) { return block.GetPosition().x == 0; }))
        {
            for (auto &block : m_blocks)
            {
                block.Move(-blockSize, 0);
            }
        }
    }
    // Right
    else if (IsKeyPressed('D'))
    {
        if (!m_blocks.empty() && m_blocks.NoneOf([](const Block &block) { return block.GetPosition().x == blockSize * (nbBlocksWidth - 1); }))
        {
            for (auto &block : m_blocks)
            {
                block.Move(blockSize, 0);
            }
        }
    }
    // Up
    else if (IsKeyPressed('W'))
    {
        if (!m_blocks.empty() && m_blocks.NoneOf([](const Block &block) { return block.GetPosition().y == 0; }))
        {
            for (auto &block : m_blocks)
            {
                block.Move(0, -blockSize);
            }
        }
    }
    // Down
    else if (IsKeyPressed('S'))
    {
        if (!m_blocks.empty() && m_blocks.NoneOf([](const Block &block) { return block.GetPosition().y == blockSize * (nbBlocksHeight - 1); }))
        {
            for (auto &block : m_blocks)
            {
                block.Move(0, blockSize);
            }
        }
    }
}

void EditorState::Update()
{
    if (m_paclockRect.CheckCollision(rl::Mouse::GetPosition()) && rl::Mouse::IsButtonPressed(MOUSE_BUTTON_LEFT))
    {
        m_locked = !m_locked;
        this->PlaySound("selected");
    }
    
    if (m_locked)
    {
        this->UpdateButtons();
        if (m_drawText && m_textClock.GetElapsedTime() > s_drawTextTime)
        {
            m_drawText = false;
        }
        return;
    }

    this->UpdateBlockPosition();
    this->HandleKeysForBlocksMovement();

    // If you press '1' (which equals 49) the first block (wall) is chosen, and so on
    for (gsl::index key{ '1' }; key < '6'; ++key)
    {
        if (IsKeyPressed(key))
        {
            // A conversion is done here so that the sequence starts from 0
            const gsl::index i{ key - '1' };
            m_currentBlockSrcRect = m_blockTypes[i].GetTextureRect();
            m_blockTypeRect.SetPosition(m_blockTypes[i].GetPosition().x - 2, m_blockTypes[i].GetPosition().y - 2);
            this->PlaySound("selected");
        }
    }
    
    const auto *const blockTypeIt{ 
        std::find_if(
            m_blockTypes.begin(), m_blockTypes.end(), 
            [this](const auto &blockType) { 
                return rl::Rectangle{ blockType.GetPosition(), m_blockTypeRect.GetSize() }.CheckCollision(rl::Mouse::GetPosition());
            }
        ) 
    };

    if (rl::Mouse::IsButtonPressed(MOUSE_BUTTON_LEFT) && blockTypeIt != m_blockTypes.end())
    {
        m_currentBlockSrcRect = blockTypeIt->GetTextureRect();
        m_blockTypeRect.SetPosition(blockTypeIt->GetPosition().x - 2, blockTypeIt->GetPosition().y - 2);
        this->PlaySound("selected");
    }

    if (rl::Mouse::GetY() > blockSize * nbBlocksHeight)
    {
        return;
    }

    const auto it{ m_blocks.Find(m_currentBlockPos) };

    if (rl::Mouse::IsButtonDown(MOUSE_BUTTON_LEFT) && (m_currentBlockSrcRect != m_assetsManager.GetRect("player_down") || 
        m_blocks.NoneOf([this](const auto &block){ return block.GetTextureRect() == m_assetsManager.GetRect("player_down"); })))
    {
        if (IsKeyDown(KEY_LEFT_CONTROL))
        {
            it->SetFillColor(rl::Color::Red().Alpha(0.5f));
        }
        else if (it != m_blocks.end())
        {
            it->SetTextureRect(m_currentBlockSrcRect);
        }
        else
        {
            m_blocks.emplace_back(m_currentBlockSrcRect, m_currentBlockPos);
        }
    }
    else if (rl::Mouse::IsButtonDown(MOUSE_BUTTON_RIGHT) && it != m_blocks.end())
    {
        if (IsKeyDown(KEY_LEFT_CONTROL))
        {
            it->SetFillColor(rl::Color::White());
        }
        else
        {
            m_blocks.erase(it);
        }
    }

    if (IsKeyPressed(KEY_DELETE))
    {
        const auto pred{ [](const auto &block) { return block.IsSelected(); } };
        if (m_blocks.AnyOf(pred))
        {
            m_blocks.EraseIf(pred);
        }
    }
}

void EditorState::Draw()
{
    m_blocks.Draw();

    if (m_locked)
    {
        m_assetsManager.GetButton("back").Draw();
        m_assetsManager.GetButton("delete").Draw();
        m_assetsManager.GetButton("save").Draw();
        m_assetsManager.GetButton("reset").Draw();

        if (m_drawText)
        {
            this->DrawText(
                m_text, 20, { 
                    (m_window.GetWidth() - m_assetsManager.GetFont("main").MeasureText(m_text, 20, 0).x) / 2.f, 
                    (m_window.GetHeight() - 20) * 0.75f 
                }
            );
        }
    }
    else
    {
        rl::Color::White().DrawRectangleLines({ m_currentBlockPos.x - 2, m_currentBlockPos.y - 2, blockSize + 4.f, blockSize + 4.f }, 2);
        m_assetsManager.GetTexture("spritesheet").Draw(m_currentBlockSrcRect, m_currentBlockPos);
    }

    rl::Color::White().DrawRectangleLines(m_blockTypeRect, 2);

    for (auto &blockType : m_blockTypes)
    {
        m_assetsManager.GetTexture("spritesheet").DrawTiled(
            blockType.GetTextureRect(), 
            rl::Rectangle{ blockType.GetPosition(), { blockType.GetTextureRect().width * 0.75f, blockType.GetTextureRect().height * 0.75f } }, 
            { 0.f, 0.f }, 0, 0.75f
        );
    }
    
    m_assetsManager.GetTexture("spritesheet").Draw(m_assetsManager.GetRect(m_locked ? "locked" : "unlocked"), m_padlockPos);
}
