#include "Configuration.h"

#include <toml++/toml.hpp>

#include "Util/TOML.h"

void Configuration::Init()
{
    auto config = GetSingleton();
    if (std::filesystem::exists(path)) {
        config->Load();
    } else {
        // Export default config if config file not exists.
        config->Save();
    }
}

void Configuration::Load()
{
    try {
        LoadImpl();
        SKSE::log::info("Successfully loaded configuration from \"{}\".", path);
    } catch (const toml::parse_error& e) {
        auto msg = std::format("Failed to load configuration from \"{}\" (error occurred at line {}, column {}): {}.",
            path, e.source().begin.line, e.source().begin.column, e.what());
        SKSE::stl::report_and_fail(msg);
    } catch (const std::exception& e) {
        auto msg = std::format("Failed to load configuration from \"{}\": {}.", path, e.what());
        SKSE::stl::report_and_fail(msg);
    }
}

void Configuration::Save() const
{
    try {
        SaveImpl();
        SKSE::log::info("Successfully saved configuration to \"{}\".", path);
    } catch (const std::exception& e) {
        auto msg = std::format("Failed to save configuration to \"{}\": {}.", path, e.what());
        SKSE::stl::report_and_fail(msg);
    }
}

void Configuration::LoadImpl()
{
    auto data = toml::parse_file(path);

    LoadTOMLValue(data, "iHotkey"sv, iHotkey);
    LoadTOMLValue(data, "iModifier"sv, iModifier);

    LoadTOMLValue(data, "iHotkeyCompass"sv, iHotkeyCompass);
    LoadTOMLValue(data, "iModifierCompass"sv, iModifierCompass);

    LoadTOMLValue(data, "iHotkeySubtitle"sv, iHotkeySubtitle);
    LoadTOMLValue(data, "iModifierSubtitle"sv, iModifierSubtitle);

    LoadTOMLValue(data, "slHUDNames"sv, slHUDNames);
    LoadTOMLValue(data, "slMenuNames"sv, slMenuNames);
    LoadTOMLValue(data, "slBannedMenuNames"sv, slBannedMenuNames);

    // Make sure menu names are ordered, so that binary search can be used.
    std::ranges::sort(slMenuNames);
    std::ranges::sort(slBannedMenuNames);
}

void Configuration::SaveImpl() const
{
    toml::table data;

    SaveTOMLValue(data, "iHotkey"sv, iHotkey);
    SaveTOMLValue(data, "iModifier"sv, iModifier);

    SaveTOMLValue(data, "iHotkeyCompass"sv, iHotkeyCompass);
    SaveTOMLValue(data, "iModifierCompass"sv, iModifierCompass);

    SaveTOMLValue(data, "iHotkeySubtitle"sv, iHotkeySubtitle);
    SaveTOMLValue(data, "iModifierSubtitle"sv, iModifierSubtitle);

    SaveTOMLValue(data, "slHUDNames"sv, slHUDNames);
    SaveTOMLValue(data, "slMenuNames"sv, slMenuNames);
    SaveTOMLValue(data, "slBannedMenuNames"sv, slBannedMenuNames);

    std::ofstream file{ path };
    file << data << std::endl;
}
