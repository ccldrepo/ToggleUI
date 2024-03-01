#include "Application.h"

Application* Application::GetSingleton()
{
    static Application singleton;
    return std::addressof(singleton);
}

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
