#include "Function.h"

#include "Application.h"
#include "Configuration.h"

MFMAPI void ToggleUI()
{
    auto app = Application::GetSingleton();
    app->ToggleUI();
}

MFMAPI void ToggleCompass()
{
    auto app = Application::GetSingleton();
    app->ToggleCompass();
}

MFMAPI void ToggleSubtitle()
{
    auto app = Application::GetSingleton();
    app->ToggleSubtitle();
}

MFMAPI void ReloadConfig(char* a_msg, size_t a_len)
{
    try {
        Configuration::Init(true);
    } catch (const std::exception& e) {
        std::memcpy(a_msg, e.what(), std::min(std::strlen(e.what()) + 1, a_len));
    }
}
