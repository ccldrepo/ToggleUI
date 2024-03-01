#pragma once

#include <PCH.h>

class Configuration
{
public:
    static Configuration* GetSingleton();
    Configuration(const Configuration&) = delete;
    Configuration& operator=(const Configuration&) = delete;

    void Init();
    void Load();
    void Save() const;

    struct Default
    {
        static constexpr uint32_t iHotkey{ RE::DirectInput8::DIK_F6 };
    };

    uint32_t iHotkey{ Default::iHotkey };

private:
    Configuration() = default;

    void LoadImpl();
    void SaveImpl() const;

    mutable std::mutex mutex;
    const std::string  path{ "Data/SKSE/Plugins/ccld_ToggleUI.toml"sv };
};
