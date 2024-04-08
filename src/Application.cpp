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
        auto controlMap = RE::ControlMap::GetSingleton();
        if (controlMap->contextPriorityStack.empty()) {
            return false;
        }
        return controlMap->contextPriorityStack.back() == RE::UserEvents::INPUT_CONTEXT_ID::kGameplay;
    }
}

bool Application::IsMenu(std::string_view a_menuName) const
{
    for (std::string_view menuName : config->slMenuNames) {
        if (menuName == a_menuName) {
            return true;
        }
    }
    return false;
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

void Application::ToggleCompass() const
{
    const char* path = "_root.HUDMovieBaseInstance.CompassShoutMeterHolder._alpha";

    auto ui = RE::UI::GetSingleton();
    auto uiMovie = ui->GetMovieView(RE::HUDMenu::MENU_NAME);

    double value = uiMovie->GetVariableDouble(path);
    SKSE::log::info("compass: {}", value);
    uiMovie->SetVariableDouble(path, value < 1.0 ? 100.0 : 0.0);
}

void Application::ToggleSubtitle() const
{
    const char* path = "_root.HUDMovieBaseInstance.SubtitleTextHolder._alpha";

    auto ui = RE::UI::GetSingleton();
    auto uiMovie = ui->GetMovieView(RE::HUDMenu::MENU_NAME);

    double value = uiMovie->GetVariableDouble(path);
    SKSE::log::info("subtitle: {}", value);
    uiMovie->SetVariableDouble(path, value < 1.0 ? 100.0 : 0.0);
}

bool Application::IsInMenu(RE::UI* a_ui) const  //
{
    return IsAnyOfMenuOpen(a_ui, config->slMenuNames);
}

bool Application::IsInBannedMenu(RE::UI* a_ui) const  //
{
    return IsAnyOfMenuOpen(a_ui, config->slBannedMenuNames);
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
