#include "Application.h"

bool Application::IsMenu(std::string_view a_menuName)
{
    for (std::string_view menuName : config->slMenuNames) {
        if (menuName == a_menuName) {
            return true;
        }
    }
    return false;
}

void Application::ResetUI()
{
    if (!menuVisible) {
        auto ui = RE::UI::GetSingleton();
        ToggleMenu(ui);
    }
}

void Application::ToggleUI()
{
    auto ui = RE::UI::GetSingleton();
    if (IsInMenu(ui)) {
        ToggleMenu(ui);
    } else {
        ToggleHUD(ui);
    }
}

bool Application::IsInMenu(RE::UI* ui)
{
    for (std::string_view menuName : config->slMenuNames) {
        if (ui->IsMenuOpen(menuName)) {
            return true;
        }
    }
    return false;
}

void Application::ToggleHUD(RE::UI* ui)
{
    hudVisible = !hudVisible;
    RE::GFxValue value{ hudVisible ? 100 : 0 };
    for (std::string_view hudName : config->slHUDNames) {
        if (auto uiMovie = ui->GetMovieView(hudName)) {
            uiMovie->SetVariable("_root._alpha", value);
        }
    }
    RE::PlaySound("UIMenuFocus");
}

void Application::ToggleMenu(RE::UI* ui)
{
    // Toggle the whole UI system.
    menuVisible = !menuVisible;
    ui->ShowMenus(menuVisible);
    RE::PlaySound("UIMenuFocus");
}
