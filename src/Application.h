#pragma once

#include <PCH.h>

#include "Util/Singleton.h"

class Application final : public Singleton<Application>
{
    friend class Singleton<Application>;

public:
    static bool IsMenu(std::string_view a_menuName);
    void        ResetUI();
    void        ToggleUI();

    static constexpr std::array hudNames{
        RE::HUDMenu::MENU_NAME,
        "MiniMapMenu"sv,
        "oxygenMeter"sv,
        "oxygenMeter2"sv,
        "TrueHUD"sv,
    };

    static constexpr std::array menuNames{
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

private:
    Application() = default;

    static bool IsInMenu(RE::UI* ui);
    void        ToggleHUD(RE::UI* ui);
    void        ToggleMenu(RE::UI* ui);

    bool hudVisible{ true };
    bool menuVisible{ true };
};
