#include "Application.h"

namespace
{
    inline bool IsAnyOfMenuOpen(RE::UI* a_ui, const std::vector<std::string>& a_menuNames)
    {
        for (std::string_view menuName : a_menuNames) {
            if (a_ui->IsMenuOpen(menuName)) {
                return true;
            }
        }
        return false;
    }

    inline bool IsInGameplayContext()
    {
        // It seems like ControlMap is broken on AE.
        if (REL::Module::IsAE()) {
            return false;
        }
        auto controlMap = RE::ControlMap::GetSingleton();
        if (controlMap->contextPriorityStack.empty()) {
            return false;
        }
        return controlMap->contextPriorityStack.back() == RE::UserEvents::INPUT_CONTEXT_ID::kGameplay;
    }
}

bool Application::IsMenu(std::string_view a_menuName) const
{
    return std::ranges::binary_search(config->slMenuNames, a_menuName);
}

void Application::ToggleUI()
{
    auto ui = RE::UI::GetSingleton();
    if (IsInGameplayContext()) {
        ToggleHUD(ui);
        RE::PlaySound("UIMenuFocus");
    } else if (IsInBannedMenu(ui)) {
        // Disable toggle.
    } else if (IsInMenu(ui)) {
        ToggleMenu(ui);
        RE::PlaySound("UIMenuFocus");
    } else {
        ToggleHUD(ui);
        RE::PlaySound("UIMenuFocus");
    }
}

void Application::ResetUI()
{
    if (!menuVisible) {
        auto ui = RE::UI::GetSingleton();
        ToggleMenu(ui);
    }
}

void Application::ToggleCompass() const  //
{
    ToggleHUDElement("_root.HUDMovieBaseInstance.CompassShoutMeterHolder._alpha");
}

void Application::ToggleSubtitle() const  //
{
    ToggleHUDElement("_root.HUDMovieBaseInstance.SubtitleTextHolder._alpha");
}

bool Application::IsInMenu(RE::UI* a_ui) const  //
{
    return IsAnyOfMenuOpen(a_ui, config->slMenuNames);
}

bool Application::IsInBannedMenu(RE::UI* a_ui) const  //
{
    return IsAnyOfMenuOpen(a_ui, config->slBannedMenuNames);
}

bool Application::IsInMenuContext(RE::UI* a_ui) const
{
    return !IsInGameplayContext() && (IsInMenu(a_ui) || IsInBannedMenu(a_ui));
}

void Application::ToggleHUD(RE::UI* a_ui)
{
    hudVisible = !hudVisible;
    RE::GFxValue value{ hudVisible ? 100 : 0 };
    for (std::string_view hudName : config->slHUDNames) {
        if (auto uiMovie = a_ui->GetMovieView(hudName)) {
            uiMovie->SetVariable("_root._alpha", value);
        }
    }
}

void Application::ToggleMenu(RE::UI* a_ui)
{
    menuVisible = !menuVisible;
    a_ui->ShowMenus(menuVisible);
}

void Application::ToggleHUDElement(const char* a_pathToVar) const
{
    auto ui = RE::UI::GetSingleton();
    if (IsInMenuContext(ui)) {
        return;
    }

    if (auto uiMovie = ui->GetMovieView(RE::HUDMenu::MENU_NAME)) {
        double value = uiMovie->GetVariableDouble(a_pathToVar);
        uiMovie->SetVariableDouble(a_pathToVar, value < 1.0 ? 100.0 : 0.0);
        RE::PlaySound("UIMenuFocus");
    }
}
