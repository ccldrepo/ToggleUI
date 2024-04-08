#include "Hotkey.h"

#include "Application.h"
#include "Configuration.h"

namespace
{
    uint32_t RemapKey(uint32_t a_key, RE::INPUT_DEVICE a_device)
    {
        switch (a_device) {
        case RE::INPUT_DEVICE::kKeyboard:
            break;
        case RE::INPUT_DEVICE::kMouse:
            a_key += SKSE::InputMap::kMacro_MouseButtonOffset;
            break;
        case RE::INPUT_DEVICE::kGamepad:
            a_key = SKSE::InputMap::GamepadMaskToKeycode(a_key);
            break;
        default:
            break;
        }
        return a_key;
    }

    struct HotkeyContext
    {
        explicit HotkeyContext(uint32_t a_targetKey) : targetKey(a_targetKey) {}

        void Update(const RE::ButtonEvent* a_button);

        const uint32_t targetKey;

        bool hasKey{ false };
    };

    void HotkeyContext::Update(const RE::ButtonEvent* a_button)
    {
        if (!a_button || !a_button->HasIDCode()) {
            return;
        }

        if (a_button->IsDown()) {
            uint32_t key = RemapKey(a_button->GetIDCode(), a_button->GetDevice());
            if (key == targetKey) {
                hasKey = true;
            }
        }
    }
}

void HotkeyManager::Process(const RE::InputEvent* const* a_event)
{
    const auto config = Configuration::GetSingleton();

    HotkeyContext ctx{ config->iHotkey };

    for (auto event = *a_event; event; event = event->next) {
        if (auto button = event->AsButtonEvent()) {
            ctx.Update(button);
        }
    }

    if (ctx.hasKey) {
        auto app = Application::GetSingleton();
        app->ToggleUI();
    }
}
