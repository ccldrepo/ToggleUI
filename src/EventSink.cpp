#include "EventSink.h"

#include "Application.h"
#include "Configuration.h"

void InputEventSink::Register()
{
    auto manager = RE::BSInputDeviceManager::GetSingleton();
    manager->AddEventSink(GetSingleton());
    SKSE::log::info("Registered for input event.");
}

RE::BSEventNotifyControl InputEventSink::ProcessEvent(const Event* a_event, [[maybe_unused]] EventSource*)
{
    if (!a_event) {
        return RE::BSEventNotifyControl::kContinue;
    }

    auto player = RE::PlayerCharacter::GetSingleton();
    if (!player || !player->Is3DLoaded()) {
        return RE::BSEventNotifyControl::kContinue;
    }

    for (auto event = *a_event; event; event = event->next) {
        auto button = event->AsButtonEvent();
        if (!button || !button->HasIDCode() || !button->IsDown()) {
            continue;
        }

        auto config = Configuration::GetSingleton();
        if (button->GetIDCode() == config->iHotkey) {
            Application::ToggleUI();
        }
    }
    return RE::BSEventNotifyControl::kContinue;
}

void MenuOpenCloseEventSink::Register()
{
    auto ui = RE::UI::GetSingleton();
    ui->AddEventSink(GetSingleton());
    SKSE::log::info("Registered for menu open close event.");
}

static constexpr std::array activeMenuNames{
    RE::BookMenu::MENU_NAME,
    RE::Console::MENU_NAME,
    RE::InventoryMenu::MENU_NAME,
    RE::JournalMenu::MENU_NAME,
    RE::LockpickingMenu::MENU_NAME,
    RE::MagicMenu::MENU_NAME,
    RE::MapMenu::MENU_NAME,
    RE::RaceSexMenu::MENU_NAME,
    RE::StatsMenu::MENU_NAME,
    RE::TweenMenu::MENU_NAME,
};

RE::BSEventNotifyControl MenuOpenCloseEventSink::ProcessEvent(const Event* a_event, [[maybe_unused]] EventSource*)
{
    if (!a_event) {
        return RE::BSEventNotifyControl::kContinue;
    }

    if (a_event->opening) {
        for (std::string_view activeMenuName : activeMenuNames) {
            if (a_event->menuName == activeMenuName) {
                Application::ResetUI();
                break;
            }
        }
    }
    return RE::BSEventNotifyControl::kContinue;
}
