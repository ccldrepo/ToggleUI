#include "Hotkey.h"

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
}

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
