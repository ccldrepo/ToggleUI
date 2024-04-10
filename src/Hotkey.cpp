#include "Hotkey.h"

#include "Application.h"
#include "Configuration.h"

namespace
{
    std::uint32_t RemapKey(std::uint32_t a_key, RE::INPUT_DEVICE a_device)
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
        constexpr HotkeyContext(std::uint32_t a_targetHotkey, std::uint32_t a_targetModifierCompass,
            std::uint32_t a_targetModifierSubtitle) noexcept :
            targetHotkey(a_targetHotkey),
            targetModifierCompass(a_targetModifierCompass), targetModifierSubtitle(a_targetModifierSubtitle)
        {}

        void Update(const RE::ButtonEvent* a_button);

        const std::uint32_t targetHotkey;
        const std::uint32_t targetModifierCompass;
        const std::uint32_t targetModifierSubtitle;

        bool hasHotkey{ false };
        bool hasModifierCompass{ false };
        bool hasModifierSubtitle{ false };
    };

    void HotkeyContext::Update(const RE::ButtonEvent* a_button)
    {
        if (!a_button || !a_button->HasIDCode()) {
            return;
        }

        if (a_button->IsPressed()) {
            auto key = RemapKey(a_button->GetIDCode(), a_button->GetDevice());
            if (key == targetModifierCompass) {
                hasModifierCompass = true;
            } else if (key == targetModifierSubtitle) {
                hasModifierSubtitle = true;
            } else if (a_button->IsDown()) {
                if (key == targetHotkey) {
                    hasHotkey = true;
                }
            }
        }
    }
}

void HotkeyManager::Process(const RE::InputEvent* const* a_event)
{
    const auto config = Configuration::GetSingleton();

    HotkeyContext ctx{ config->iHotkey, config->iModifierCompass, config->iModifierSubtitle };

    for (auto event = *a_event; event; event = event->next) {
        switch (event->GetEventType()) {
        case RE::INPUT_EVENT_TYPE::kButton:
            ctx.Update(static_cast<const RE::ButtonEvent*>(event));
            break;
        default:
            break;
        }
    }

    if (ctx.hasHotkey) {
        auto app = Application::GetSingleton();
        if (ctx.hasModifierCompass) {
            app->ToggleCompass();
        } else if (ctx.hasModifierSubtitle) {
            app->ToggleSubtitle();
        } else {
            app->ToggleUI();
        }
    }
}
