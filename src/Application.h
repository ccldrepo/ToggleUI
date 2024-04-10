#pragma once

#include <PCH.h>

#include "Configuration.h"
#include "Util/Singleton.h"

class Application final : public Singleton<Application>
{
    friend class Singleton<Application>;

public:
    bool IsMenu(std::string_view a_menuName) const;

    void ToggleUI();
    void ResetUI();

    void ToggleCompass() const;
    void ToggleSubtitle() const;

private:
    Application() : config(Configuration::GetSingleton()) {}

    bool IsInMenu(RE::UI* a_ui) const;
    bool IsInBannedMenu(RE::UI* a_ui) const;
    bool IsInMenuContext(RE::UI* a_ui) const;

    void ToggleHUD(RE::UI* a_ui);
    void ToggleMenu(RE::UI* a_ui);

    void ToggleHUDElement(const char* a_pathToVar) const;

    bool hudVisible{ true };
    bool menuVisible{ true };

    const Configuration* config;
};
