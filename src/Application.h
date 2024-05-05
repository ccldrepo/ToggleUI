#pragma once

#include <PCH.h>

#include "Configuration.h"
#include "Util/Singleton.h"

class Application final : public Singleton<Application>
{
    friend class Singleton<Application>;

public:
    static bool IsMenu(std::string_view a_menuName);

    void ToggleUI();
    void ResetUI();

    static void ToggleCompass();
    static void TogglePlayerBar();
    static void ToggleSubtitle();

private:
    Application() = default;

    static bool IsInMenu(const Configuration* a_config, RE::UI* a_ui);
    static bool IsInBannedMenu(const Configuration* a_config, RE::UI* a_ui);
    static bool IsInMenuContext(const Configuration* a_config, RE::UI* a_ui);

    static void ToggleHUDElement(const char* a_pathToVar);

    void ToggleHUD(RE::UI* a_ui);
    void ToggleMenu(RE::UI* a_ui);

    bool _hudVisible{ true };
    bool _menuVisible{ true };
};
