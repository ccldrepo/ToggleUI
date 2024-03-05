#pragma once

#include <PCH.h>

#include "Util/Singleton.h"

class Configuration final : public Singleton<Configuration>
{
    friend class Singleton<Configuration>;

public:
    void Init();
    void Load();
    void Save() const;

    struct Default
    {
        static constexpr uint32_t iHotkey{ RE::DirectInput8::DIK_F6 };

        static constexpr std::array slHUDNames{
            RE::HUDMenu::MENU_NAME,
            "BTPS Menu"sv,
            "CastingBar"sv,
            "Floating Damage V2"sv,
            "MiniMapMenu"sv,
            "oxygenMeter2"sv,
            "TrueHUD"sv,
        };

        static constexpr std::array slMenuNames{
            RE::BarterMenu::MENU_NAME,
            RE::BookMenu::MENU_NAME,
            RE::Console::MENU_NAME,
            RE::ContainerMenu::MENU_NAME,
            RE::CraftingMenu::MENU_NAME,
            RE::DialogueMenu::MENU_NAME,
            RE::FavoritesMenu::MENU_NAME,
            RE::GiftMenu::MENU_NAME,
            RE::InventoryMenu::MENU_NAME,
            RE::JournalMenu::MENU_NAME,
            RE::LockpickingMenu::MENU_NAME,
            RE::MagicMenu::MENU_NAME,
            RE::MapMenu::MENU_NAME,
            RE::RaceSexMenu::MENU_NAME,
            RE::SleepWaitMenu::MENU_NAME,
            RE::StatsMenu::MENU_NAME,
            RE::TweenMenu::MENU_NAME,
        };
    };

    uint32_t                 iHotkey{ Default::iHotkey };
    std::vector<std::string> slHUDNames{ Default::slHUDNames.begin(), Default::slHUDNames.end() };
    std::vector<std::string> slMenuNames{ Default::slMenuNames.begin(), Default::slMenuNames.end() };

private:
    Configuration() = default;

    void LoadImpl();
    void SaveImpl() const;

    mutable std::mutex mutex;
    const std::string  path{ "Data/SKSE/Plugins/ccld_ToggleUI.toml"sv };
};
