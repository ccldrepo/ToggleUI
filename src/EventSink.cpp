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

    if (!a_event->opening) {
        if (Application::IsMenu(a_event->menuName)) {
            auto app = Application::GetSingleton();
            app->ResetUI();
        }
    }
    return RE::BSEventNotifyControl::kContinue;
}
