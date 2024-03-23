#pragma once

#include <cstdint>

#include <RE/Skyrim.h>

class HotkeyContext
{
public:
    explicit HotkeyContext(uint32_t a_targetKey) : targetKey(a_targetKey) {}

    void Update(const RE::ButtonEvent* a_button);

    const uint32_t targetKey;

    bool hasKey{ false };
};
