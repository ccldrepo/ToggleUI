#pragma once

#include <PCH.h>

#include "Configuration.h"
#include "Util/Singleton.h"

class HotkeyContext
{
public:
    explicit HotkeyContext(uint32_t a_targetKey) : targetKey(a_targetKey) {}

    void Update(const RE::ButtonEvent* button);

    const uint32_t targetKey;

    bool hasKey{ false };
};

class Application final : public Singleton<Application>
{
    friend class Singleton<Application>;

public:
    HotkeyContext NewHotkeyContext() const { return HotkeyContext{ config->iHotkey }; }

    bool IsMenu(std::string_view a_menuName) const;

    void ResetUI();
    void ToggleUI();

private:
    Application() : config(Configuration::GetSingleton()) {}

    bool IsInMenu(RE::UI* ui) const;
    bool IsInBannedMenu(RE::UI* ui) const;

    void ToggleHUD(RE::UI* ui);
    void ToggleMenu(RE::UI* ui);

    bool hudVisible{ true };
    bool menuVisible{ true };

    Configuration* config;
};
