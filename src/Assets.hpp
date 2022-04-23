#ifndef SRC_ASSETS_HPP
#define SRC_ASSETS_HPP

template<typename T>
using Dictionary = std::map<std::string, T>;

template<typename T>
class Assets
{
public:
    Assets(const std::string &p_folder, const std::string &p_extension="") : m_folder{ p_folder }, m_extension{ p_extension } { }
    void Add(const std::string &p_id, const std::string &p_fileName="")
    {
        if (!this->Exists(p_id))
        {
            if (!std::is_same_v<T, rl::Music>)
            {
                m_assets.emplace(p_id, m_folder + (p_fileName == "" ? p_id : p_fileName) + m_extension);
            }
            else
            {
                m_assets.emplace(p_id, m_folder + (p_fileName.starts_with('.') ? p_id + p_fileName : p_fileName));
            }
        }
    }

    bool Exists(const std::string &p_id) const noexcept
    {
        return m_assets.find(p_id) != m_assets.end();
    }

    const T &Get(const std::string &p_id) const { return m_assets.at(p_id); }
    T &Get(const std::string &p_id)             { return m_assets.at(p_id); }

private:
    const std::string m_folder;
    const std::string m_extension;
    Dictionary<T> m_assets;
};


#endif // SRC_ASSETS_HPP
