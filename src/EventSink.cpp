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

        uint32_t key = button->GetIDCode();
        switch (button->GetDevice()) {
        case RE::INPUT_DEVICE::kKeyboard:
            break;
        case RE::INPUT_DEVICE::kMouse:
            key += SKSE::InputMap::kMacro_MouseButtonOffset;
            break;
        case RE::INPUT_DEVICE::kGamepad:
            key = SKSE::InputMap::GamepadMaskToKeycode(key);
            break;
        default:
            break;
        }

        auto config = Configuration::GetSingleton();
        if (key == config->iHotkey) {
            auto app = Application::GetSingleton();
            app->ToggleUI();
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

RE::BSEventNotifyControl MenuOpenCloseEventSink::ProcessEvent(const Event* a_event, [[maybe_unused]] EventSource*)
{
    if (!a_event) {
        return RE::BSEventNotifyControl::kContinue;
    }

#ifdef _DEBUG
    auto ui = RE::UI::GetSingleton();

    std::vector<std::string_view> menuNames;
    std::vector<std::string_view> menuNamesOnStack;
    for (auto& entry : ui->menuMap) {
        menuNames.push_back(entry.first);
        auto& menu = entry.second.menu;
        if (menu && menu->OnStack()) {
            menuNamesOnStack.push_back(entry.first);
        }
    }

    SKSE::log::debug("Menus: {}.", menuNames);
    SKSE::log::debug("Menus On Stack: {}.", menuNamesOnStack);
    SKSE::log::debug("{} {}.", a_event->opening ? "Open" : "Close", a_event->menuName.c_str());
#endif

    if (!a_event->opening) {
        auto app = Application::GetSingleton();
        if (app->IsMenu(a_event->menuName)) {
            app->ResetUI();
        }
    }
    return RE::BSEventNotifyControl::kContinue;
}
