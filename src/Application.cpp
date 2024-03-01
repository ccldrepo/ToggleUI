#include "Application.h"

void Application::ResetUI()
{
    auto ui = RE::UI::GetSingleton();
    ui->ShowMenus(true);
}

void Application::ToggleUI()
{
    auto ui = RE::UI::GetSingleton();
    ui->ShowMenus(!ui->IsShowingMenus());
    RE::PlaySound("UIMenuFocus");
}
