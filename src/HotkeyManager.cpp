#include "HotkeyManager.h"

#include "Application.h"
#include "Configuration.h"

namespace
{
    inline std::uint32_t RemapKey(std::uint32_t a_key, RE::INPUT_DEVICE a_device)
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

    class KeyCombo
    {
    public:
        constexpr KeyCombo(std::uint32_t a_targetHotkey, std::uint32_t a_targetModifier) noexcept :
            targetHotkey(a_targetHotkey), targetModifier(a_targetModifier)
        {}

        bool IsActive() const noexcept  //
        {
            return (hasHotkey && hasModifier) || (hasHotkey && targetModifier == 0);
        }

        void UpdateDown(std::uint32_t a_key) noexcept
        {
            if (targetHotkey != 0 && a_key == targetHotkey) {
                hasHotkey = true;
            }
        }

        void UpdatePressed(std::uint32_t a_key) noexcept
        {
            if (targetModifier != 0 && a_key == targetModifier) {
                hasModifier = true;
            }
        }

    private:
        const std::uint32_t targetHotkey;
        const std::uint32_t targetModifier;

        bool hasHotkey{ false };
        bool hasModifier{ false };
    };

    class HotkeyContext
    {
    public:
        HotkeyContext(const Configuration* config) :
            hotkey(config->iHotkey, config->iModifier), hotkeyCompass(config->iHotkeyCompass, config->iModifierCompass),
            hotkeySubtitle(config->iHotkeySubtitle, config->iModifierSubtitle)
        {}

        void Update(const RE::ButtonEvent* a_button)
        {
            if (!a_button->HasIDCode()) {
                return;
            }

            if (a_button->IsPressed()) {
                auto key = RemapKey(a_button->GetIDCode(), a_button->GetDevice());

                hotkey.UpdatePressed(key);
                hotkeyCompass.UpdatePressed(key);
                hotkeySubtitle.UpdatePressed(key);

                if (a_button->IsDown()) {
                    hotkey.UpdateDown(key);
                    hotkeyCompass.UpdateDown(key);
                    hotkeySubtitle.UpdateDown(key);
                }
            }
        }

        void Finalize()
        {
            auto app = Application::GetSingleton();

            bool lpt = false;  // low priority toggle

            if (hotkeyCompass.IsActive()) {
                app->ToggleCompass();
                lpt = true;
            }

            if (hotkeySubtitle.IsActive()) {
                app->ToggleSubtitle();
                lpt = true;
            }

            if (!lpt && hotkey.IsActive()) {
                app->ToggleUI();
            }
        }

    private:
        KeyCombo hotkey;
        KeyCombo hotkeyCompass;
        KeyCombo hotkeySubtitle;
    };
}

void HotkeyManager::Process(const RE::InputEvent* const* a_event)
{
    const auto config = Configuration::GetSingleton();

    HotkeyContext ctx{ config };

    for (auto event = *a_event; event; event = event->next) {
        if (auto button = event->AsButtonEvent()) {
            ctx.Update(button);
        }
    }

    ctx.Finalize();
}
