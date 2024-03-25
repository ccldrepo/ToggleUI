#pragma once

#include <PCH.h>

#include "Configuration.h"
#include "Hotkey.h"
#include "Util/Singleton.h"

class Application final : public Singleton<Application>
{
    friend class Singleton<Application>;

public:
    HotkeyContext NewHotkeyContext() const  //
    {
        return HotkeyContext{ config->iHotkey };
    }

    bool IsMenu(std::string_view a_menuName) const;

    void ResetUI();
    void ToggleUI();

private:
    Application() : config(Configuration::GetSingleton()) {}

    bool IsInMenu(RE::UI* a_ui) const;
    bool IsInBannedMenu(RE::UI* a_ui) const;

    void ToggleHUD(RE::UI* a_ui);
    void ToggleMenu(RE::UI* a_ui);

    bool hudVisible{ true };
    bool menuVisible{ true };

    Configuration* config;
};
