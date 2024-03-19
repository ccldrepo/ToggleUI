#include "Application.h"

void HotkeyContext::Update(const RE::ButtonEvent* button)
{
    if (!button || !button->HasIDCode()) {
        return;
    }

    if (button->IsDown()) {
        uint32_t key = button->GetIDCode();
        switch (button->GetDevice()) {
        case RE::INPUT_DEVICE::kKeyboard:
            break;
        case RE::INPUT_DEVICE::kMouse:
            key += SKSE::InputMap::kMacro_MouseButtonOffset;
            break;
        case RE::INPUT_DEVICE::kGamepad:
            key = SKSE::InputMap::GamepadMaskToKeycode(key);
            break;
        default:
            break;
        }

        if (key == targetKey) {
            hasKey = true;
        }
    }
}

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
    if (IsInBannedMenu(ui)) {
        // Disable toggle.
    } else if (IsInMenu(ui)) {
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

bool Application::IsInBannedMenu(RE::UI* ui)
{
    for (std::string_view menuName : config->slBannedMenuNames) {
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
