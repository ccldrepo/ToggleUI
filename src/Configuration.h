#pragma once

#include <PCH.h>

class Configuration
{
public:
    static const Configuration* GetSingleton() { return config.get(); }

    static void Init(bool a_abort = true);

    struct Default
    {
        static constexpr std::uint32_t iHotkey{ REX::W32::DIK_F6 };
        static constexpr std::uint32_t iModifier{ 0 };

        static constexpr std::uint32_t iHotkeyCompass{ REX::W32::DIK_F6 };
        static constexpr std::uint32_t iModifierCompass{ REX::W32::DIK_RSHIFT };

        static constexpr std::uint32_t iHotkeySubtitle{ REX::W32::DIK_F6 };
        static constexpr std::uint32_t iModifierSubtitle{ REX::W32::DIK_RCONTROL };

        static constexpr std::array slHUDNames{
            RE::HUDMenu::MENU_NAME,
            "BTPS Menu"sv,
            "CastingBar"sv,
            "Durability Menu"sv,
            "Floating Damage V2"sv,
            "LootMenu"sv,
            "MiniMapMenu"sv,
            "OStimAlignMenu"sv,
            "OstimSceneMenu"sv,
            "OStimSearchMenu"sv,
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
            RE::LevelUpMenu::MENU_NAME,
            RE::LockpickingMenu::MENU_NAME,
            RE::MagicMenu::MENU_NAME,
            RE::MapMenu::MENU_NAME,
            RE::RaceSexMenu::MENU_NAME,
            RE::SleepWaitMenu::MENU_NAME,
            RE::StatsMenu::MENU_NAME,
            RE::TrainingMenu::MENU_NAME,
            RE::TutorialMenu::MENU_NAME,
            RE::TweenMenu::MENU_NAME,
        };
    };

    std::uint32_t iHotkey{ Default::iHotkey };
    std::uint32_t iModifier{ Default::iModifier };

    std::uint32_t iHotkeyCompass{ Default::iHotkeyCompass };
    std::uint32_t iModifierCompass{ Default::iModifierCompass };

    std::uint32_t iHotkeySubtitle{ Default::iHotkeySubtitle };
    std::uint32_t iModifierSubtitle{ Default::iModifierSubtitle };

    std::vector<std::string> slHUDNames{ Default::slHUDNames.begin(), Default::slHUDNames.end() };
    std::vector<std::string> slMenuNames{ Default::slMenuNames.begin(), Default::slMenuNames.end() };
    std::vector<std::string> slBannedMenuNames{};

private:
    Configuration() = default;

    Configuration(const Configuration&) = delete;
    Configuration(Configuration&&) = delete;
    Configuration& operator=(const Configuration&) = delete;
    Configuration& operator=(Configuration&&) = delete;

    void Load(bool a_abort);
    void Save(bool a_abort) const;

    void LoadImpl();
    void SaveImpl() const;

    static inline std::unique_ptr<Configuration> config{ nullptr };

    static inline const std::filesystem::path path{ L"Data/SKSE/Plugins/ccld_ToggleUI.toml"sv };
};
