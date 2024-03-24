#include "EventSink.h"

#include "Application.h"

void InputEventSink::Register()
{
    auto manager = RE::BSInputDeviceManager::GetSingleton();
    manager->AddEventSink(GetSingleton());
    SKSE::log::info("Registered input event.");
}

RE::BSEventNotifyControl InputEventSink::ProcessEvent(const Event* a_event, [[maybe_unused]] EventSource*)
{
    if (!a_event) {
        return RE::BSEventNotifyControl::kContinue;
    }

    if (auto player = RE::PlayerCharacter::GetSingleton(); !player || !player->Is3DLoaded()) {
        return RE::BSEventNotifyControl::kContinue;
    }

    auto app = Application::GetSingleton();
    auto hotkey = app->NewHotkeyContext();

    for (auto event = *a_event; event; event = event->next) {
        hotkey.Update(event->AsButtonEvent());
    }

    if (hotkey.hasKey) {
        app->ToggleUI();
    }
    return RE::BSEventNotifyControl::kContinue;
}

void MenuOpenCloseEventSink::Register()
{
    auto ui = RE::UI::GetSingleton();
    ui->AddEventSink(GetSingleton());
    SKSE::log::info("Registered menu open/close event.");
}

RE::BSEventNotifyControl MenuOpenCloseEventSink::ProcessEvent(const Event* a_event, [[maybe_unused]] EventSource*)
{
    if (!a_event) {
        return RE::BSEventNotifyControl::kContinue;
    }

#ifdef _DEBUG
    auto ui = RE::UI::GetSingleton();

    std::vector<std::string_view> menuNames;
    std::vector<std::string_view> menuNamesOnStack;
    for (auto& [name, entry] : ui->menuMap) {
        menuNames.push_back(name);
        if (entry.menu && entry.menu->OnStack()) {
            menuNamesOnStack.push_back(name);
        }
    }

    SKSE::log::debug(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    SKSE::log::debug("{} {}.", a_event->opening ? "Open" : "Close", a_event->menuName.c_str());
    SKSE::log::debug("Menus: {}.", menuNames);
    SKSE::log::debug("Menus On Stack: {}.", menuNamesOnStack);
    SKSE::log::debug(
        "numPausesGame: {}, numItemMenus: {}, numDisablePauseMenu: {}, numAllowSaving: {}, "
        "numDontHideCursorWhenTopmost: {}, numCustomRendering: {}, numApplicationMenus: {}",
        ui->numPausesGame, ui->numItemMenus, ui->numDisablePauseMenu, ui->numAllowSaving,
        ui->numDontHideCursorWhenTopmost, ui->numCustomRendering, ui->numApplicationMenus);
    SKSE::log::debug("modal: {}, menuSystemVisible: {}, closingAllMenus: {}", ui->modal, ui->menuSystemVisible,
        ui->closingAllMenus);
    SKSE::log::debug("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
#endif

    if (!a_event->opening) {
        auto app = Application::GetSingleton();
        if (app->IsMenu(a_event->menuName)) {
            app->ResetUI();
        }
    }
    return RE::BSEventNotifyControl::kContinue;
}
