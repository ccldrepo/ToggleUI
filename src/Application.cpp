#include "Application.h"

#include "Configuration.h"

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
        // https://github.com/CharmedBaryon/CommonLibSSE-NG/issues/89
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

bool Application::IsMenu(std::string_view a_menuName)
{
    const auto config = Configuration::GetSingleton();
    return std::ranges::binary_search(config->slMenuNames, a_menuName);
}

void Application::ToggleUI()
{
    const auto config = Configuration::GetSingleton();
    auto       ui = RE::UI::GetSingleton();
    if (IsInGameplayContext()) {
        ToggleHUD(config, ui);
        RE::PlaySound("UIMenuFocus");
    } else if (IsInBannedMenu(config, ui)) {
        // Disable toggle.
    } else if (IsInMenu(config, ui)) {
        ToggleMenu(ui);
        RE::PlaySound("UIMenuFocus");
    } else {
        ToggleHUD(config, ui);
        RE::PlaySound("UIMenuFocus");
    }
}

void Application::ResetUI()
{
    if (!_menuVisible) {
        auto ui = RE::UI::GetSingleton();
        ToggleMenu(ui);
    }
}

void Application::ToggleCompass()  //
{
    ToggleHUDElement("_root.HUDMovieBaseInstance.CompassShoutMeterHolder._alpha");
}

void Application::TogglePlayerBar()
{
    ToggleHUDElement("_root.HUDMovieBaseInstance.Health._alpha");
    ToggleHUDElement("_root.HUDMovieBaseInstance.Magica._alpha");
    ToggleHUDElement("_root.HUDMovieBaseInstance.Stamina._alpha");
}

void Application::ToggleSubtitle()  //
{
    ToggleHUDElement("_root.HUDMovieBaseInstance.SubtitleTextHolder._alpha");
}

bool Application::IsInMenu(const Configuration* a_config, RE::UI* a_ui)
{
    return IsAnyOfMenuOpen(a_ui, a_config->slMenuNames);
}

bool Application::IsInBannedMenu(const Configuration* a_config, RE::UI* a_ui)
{
    return IsAnyOfMenuOpen(a_ui, a_config->slBannedMenuNames);
}

bool Application::IsInMenuContext(const Configuration* a_config, RE::UI* a_ui)
{
    return !IsInGameplayContext() && (IsInMenu(a_config, a_ui) || IsInBannedMenu(a_config, a_ui));
}

void Application::ToggleHUDElement(const char* a_pathToVar)
{
    const auto config = Configuration::GetSingleton();
    auto       ui = RE::UI::GetSingleton();
    if (IsInMenuContext(config, ui)) {
        return;
    }

    if (auto uiMovie = ui->GetMovieView(RE::HUDMenu::MENU_NAME)) {
        double value = uiMovie->GetVariableDouble(a_pathToVar);
        uiMovie->SetVariableDouble(a_pathToVar, value < 1.0 ? 100.0 : 0.0);
    }
}

void Application::ToggleHUD(const Configuration* a_config, RE::UI* a_ui)
{
    _hudVisible = !_hudVisible;
    RE::GFxValue value{ _hudVisible ? 100 : 0 };
    for (std::string_view hudName : a_config->slHUDNames) {
        if (auto uiMovie = a_ui->GetMovieView(hudName)) {
            uiMovie->SetVariable("_root._alpha", value);
        }
    }
}

void Application::ToggleMenu(RE::UI* a_ui)
{
    _menuVisible = !_menuVisible;
    a_ui->ShowMenus(_menuVisible);
}
