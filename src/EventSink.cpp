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

    auto controlMap = RE::ControlMap::GetSingleton();

    std::vector<uint32_t> contextPriorityStack;
    for (auto inputContext : controlMap->contextPriorityStack) {
        contextPriorityStack.push_back(inputContext);
    }

    SKSE::log::debug(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    SKSE::log::debug("{} {}.", a_event->opening ? "Open" : "Close", std::string_view{ a_event->menuName });
    SKSE::log::debug("Menus: {}.", menuNames);
    SKSE::log::debug("Menus On Stack: {}.", menuNamesOnStack);
    SKSE::log::debug("Context Priority Stack: {}.", contextPriorityStack);
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

void MenuModeChangeEventSink::Register()
{
    auto ui = RE::UI::GetSingleton();
    ui->AddEventSink(GetSingleton());
    SKSE::log::info("Registered menu mode change event.");
}

RE::BSEventNotifyControl MenuModeChangeEventSink::ProcessEvent(const Event* a_event, [[maybe_unused]] EventSource*)
{
    if (!a_event) {
        return RE::BSEventNotifyControl::kContinue;
    }

#ifdef _DEBUG
    SKSE::log::debug("Menu: {}, Mode: {}.", std::string_view{ a_event->menu }, a_event->mode.underlying());
#endif

    return RE::BSEventNotifyControl::kContinue;
}
