#pragma once

#include <PCH.h>

#include "Configuration.h"
#include "Util/Singleton.h"

class Application final : public Singleton<Application>
{
    friend class Singleton<Application>;

public:
    bool IsMenu(std::string_view a_menuName);
    void ResetUI();
    void ToggleUI();

private:
    Application() : config(Configuration::GetSingleton()) {}

    bool IsInMenu(RE::UI* ui);
    void ToggleHUD(RE::UI* ui);
    void ToggleMenu(RE::UI* ui);

    bool hudVisible{ true };
    bool menuVisible{ true };

    // Associate with current configuration.
    Configuration* config;
};
