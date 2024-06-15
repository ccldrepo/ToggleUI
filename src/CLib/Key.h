#pragma once

namespace CLib
{
    inline std::uint32_t ParseKey(std::uint32_t a_key, RE::INPUT_DEVICE a_device)
    {
        switch (a_device) {
        case RE::INPUT_DEVICE::kKeyboard:
            return a_key;
        case RE::INPUT_DEVICE::kMouse:
            return a_key + SKSE::InputMap::kMacro_MouseButtonOffset;
        case RE::INPUT_DEVICE::kGamepad:
            return SKSE::InputMap::GamepadMaskToKeycode(a_key);
        default:
            return a_key;
        }
    }

    constexpr inline std::uint32_t INVALID_KEY = 0;

    class Key
    {
    public:
        explicit Key(std::uint32_t a_targetHotkey) noexcept : targetHotkey(a_targetHotkey) {}

        bool IsActive() const noexcept  //
        {
            return hasHotkey;
        }

        void Update(std::uint32_t a_key) noexcept
        {
            if (targetHotkey != INVALID_KEY && a_key == targetHotkey) {
                hasHotkey = true;
            }
        }

    private:
        const std::uint32_t targetHotkey;

        bool hasHotkey{ false };
    };

    class KeyCombo
    {
    public:
        explicit KeyCombo(std::uint32_t a_targetHotkey) noexcept :
            targetHotkey(a_targetHotkey), targetModifier(INVALID_KEY), count(CalcCount(a_targetHotkey, INVALID_KEY))
        {}

        KeyCombo(std::uint32_t a_targetHotkey, std::uint32_t a_targetModifier) noexcept :
            targetHotkey(a_targetHotkey), targetModifier(a_targetModifier),
            count(CalcCount(a_targetHotkey, a_targetModifier))
        {}

        std::uint32_t Count() const noexcept { return count; }

        bool IsActive() const noexcept  //
        {
            return hasHotkey && (targetModifier == INVALID_KEY || hasModifier);
        }

        void UpdateDown(std::uint32_t a_key) noexcept
        {
            if (targetHotkey != INVALID_KEY && a_key == targetHotkey) {
                hasHotkey = true;
            }
        }

        void UpdatePressed(std::uint32_t a_key) noexcept
        {
            if (targetModifier != INVALID_KEY && a_key == targetModifier) {
                hasModifier = true;
            }
        }

    private:
        static constexpr std::uint16_t CalcCount(std::uint32_t a_targetHotkey, std::uint32_t a_targetModifier) noexcept
        {
            if (a_targetHotkey == INVALID_KEY) {
                return 0;
            } else if (a_targetModifier == INVALID_KEY) {
                return 1;
            } else {
                return 2;
            }
        }

        const std::uint32_t targetHotkey;
        const std::uint32_t targetModifier;

        bool hasHotkey{ false };
        bool hasModifier{ false };

        const std::uint16_t count;
    };
}
