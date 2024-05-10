#include "Function.h"

#include <spdlog/sinks/ostream_sink.h>

#include "Application.h"
#include "Configuration.h"

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

MFMAPI void ToggleUI()
{
    auto app = Application::GetSingleton();
    app->ToggleUI();
}

MFMAPI void ReloadConfig(char* a_msg, std::size_t a_len)
{
    std::ostringstream oss;

    auto sink = std::make_shared<spdlog::sinks::ostream_sink_mt>(oss);
    sink->set_pattern("[%l] %v");
    spdlog::default_logger_raw()->sinks().push_back(sink);

    try {
        Configuration::Init(false);
    } catch (const std::exception&) {
        // Suppress exception.
    }

    auto msg = oss.str();
    std::memcpy(a_msg, msg.c_str(), std::min(msg.size() + 1, a_len));
    spdlog::default_logger_raw()->sinks().pop_back();
}
