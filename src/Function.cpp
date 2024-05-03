#include "Function.h"

#include "Application.h"
#include "Configuration.h"

MFMAPI void ToggleUI()
{
    auto app = Application::GetSingleton();
    app->ToggleUI();
}

MFMAPI void ToggleHUD_Compass()
{
    auto app = Application::GetSingleton();
    app->ToggleCompass();
}

MFMAPI void ToggleHUD_PlayerBar()
{
    auto app = Application::GetSingleton();
    app->TogglePlayerBar();
}

MFMAPI void ToggleHUD_Subtitle()
{
    auto app = Application::GetSingleton();
    app->ToggleSubtitle();
}

MFMAPI void ReloadConfig(char* a_msg, size_t a_len)
{
    try {
        Configuration::Init(true);
    } catch (const SKSE::stl::log_message& e) {
        std::memcpy(a_msg, e.c_str(), std::min(e.size() + 1, a_len));
    } catch (const std::exception& e) {
        std::memcpy(a_msg, e.what(), std::min(std::strlen(e.what()) + 1, a_len));
    }
}
