#include "Configuration.h"

#include <toml++/toml.hpp>

#include "Util/TOML.h"

void Configuration::Init(bool a_abort)
{
    auto tmp = std::unique_ptr<Configuration>{ new Configuration };
    if (std::filesystem::exists(_path)) {
        tmp->Load(a_abort);
    } else {
        // Export default config if config file not exists.
        tmp->Save(a_abort);
    }
    _singleton = std::move(tmp);
}

void Configuration::Load(bool a_abort)
{
    try {
        LoadImpl();
        SKSE::log::info("Successfully loaded configuration from \"{}\".", PathToStr(_path));
    } catch (const toml::parse_error& e) {
        auto msg = std::format("Failed to load configuration from \"{}\" (error occurred at line {}, column {}): {}.",
            PathToStr(_path), e.source().begin.line, e.source().begin.column, e.what());
        SKSE::stl::report_fatal_error(msg, a_abort);
    } catch (const std::system_error& e) {
        auto msg = std::format("Failed to load configuration from \"{}\": {}.", PathToStr(_path),
            SKSE::stl::ansi_to_utf8(e.what()).value_or(e.what()));
        SKSE::stl::report_fatal_error(msg, a_abort);
    } catch (const std::exception& e) {
        auto msg = std::format("Failed to load configuration from \"{}\": {}.", PathToStr(_path), e.what());
        SKSE::stl::report_fatal_error(msg, a_abort);
    }
}

void Configuration::Save(bool a_abort) const
{
    try {
        SaveImpl();
        SKSE::log::info("Successfully saved configuration to \"{}\".", PathToStr(_path));
    } catch (const std::system_error& e) {
        auto msg = std::format("Failed to save configuration to \"{}\": {}.", PathToStr(_path),
            SKSE::stl::ansi_to_utf8(e.what()).value_or(e.what()));
        SKSE::stl::report_fatal_error(msg, a_abort);
    } catch (const std::exception& e) {
        auto msg = std::format("Failed to save configuration to \"{}\": {}.", PathToStr(_path), e.what());
        SKSE::stl::report_fatal_error(msg, a_abort);
    }
}

void Configuration::LoadImpl()
{
    auto data = LoadTOMLFile(_path);

    GetTOMLValue(data, "iHotkey"sv, iHotkey);
    GetTOMLValue(data, "iModifier"sv, iModifier);

    GetTOMLValue(data, "iHotkeyCompass"sv, iHotkeyCompass);
    GetTOMLValue(data, "iModifierCompass"sv, iModifierCompass);

    GetTOMLValue(data, "iHotkeyPlayerBar"sv, iHotkeyPlayerBar);
    GetTOMLValue(data, "iModifierPlayerBar"sv, iModifierPlayerBar);

    GetTOMLValue(data, "iHotkeySubtitle"sv, iHotkeySubtitle);
    GetTOMLValue(data, "iModifierSubtitle"sv, iModifierSubtitle);

    GetTOMLValue(data, "slHUDNames"sv, slHUDNames);
    GetTOMLValue(data, "slMenuNames"sv, slMenuNames);
    GetTOMLValue(data, "slBannedMenuNames"sv, slBannedMenuNames);

    // Make sure menu names are ordered, so that binary search can be used.
    std::ranges::sort(slMenuNames);
    std::ranges::sort(slBannedMenuNames);
}

void Configuration::SaveImpl() const
{
    toml::table data;

    SetTOMLValue(data, "iHotkey"sv, iHotkey);
    SetTOMLValue(data, "iModifier"sv, iModifier);

    SetTOMLValue(data, "iHotkeyCompass"sv, iHotkeyCompass);
    SetTOMLValue(data, "iModifierCompass"sv, iModifierCompass);

    SetTOMLValue(data, "iHotkeyPlayerBar"sv, iHotkeyPlayerBar);
    SetTOMLValue(data, "iModifierPlayerBar"sv, iModifierPlayerBar);

    SetTOMLValue(data, "iHotkeySubtitle"sv, iHotkeySubtitle);
    SetTOMLValue(data, "iModifierSubtitle"sv, iModifierSubtitle);

    SetTOMLValue(data, "slHUDNames"sv, slHUDNames);
    SetTOMLValue(data, "slMenuNames"sv, slMenuNames);
    SetTOMLValue(data, "slBannedMenuNames"sv, slBannedMenuNames);

    SaveTOMLFile(_path, data);
}
