#include "Configuration.h"

#include <toml++/toml.hpp>

#include "Util/TOML.h"

void Configuration::Init()
{
    if (std::filesystem::exists(path)) {
        Load();
    } else {
        Save();  // Export default config if config file not exists.
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

    LoadTOMLValue(data, "slHUDNames"sv, slHUDNames);
    LoadTOMLValue(data, "slMenuNames"sv, slMenuNames);
    LoadTOMLValue(data, "slBannedMenuNames"sv, slBannedMenuNames);
}

void Configuration::SaveImpl() const
{
    toml::table data;

    SaveTOMLValue(data, "iHotkey"sv, iHotkey);

    SaveTOMLValue(data, "slHUDNames"sv, slHUDNames);
    SaveTOMLValue(data, "slMenuNames"sv, slMenuNames);
    SaveTOMLValue(data, "slBannedMenuNames"sv, slBannedMenuNames);

    std::ofstream file{ path };
    file << data << std::endl;
}

const std::string Configuration::path{ "Data/SKSE/Plugins/ccld_ToggleUI.toml"sv };
