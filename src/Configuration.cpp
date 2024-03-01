#include "Configuration.h"

#include <toml++/toml.hpp>

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
    std::scoped_lock lock{ mutex };
    try {
        LoadImpl();
        SKSE::log::info("Loaded configuration from \"{}\".", path);
    } catch (const std::exception& e) {
        SKSE::log::error("Failed to load configuration from \"{}\".", path);
        SKSE::log::error("{}", e.what());
        throw;
    }
}

void Configuration::Save() const
{
    std::scoped_lock lock{ mutex };
    try {
        SaveImpl();
        SKSE::log::info("Saved configuration to \"{}\".", path);
    } catch (const std::exception& e) {
        SKSE::log::error("Failed to save configuration to \"{}\".", path);
        SKSE::log::error("{}", e.what());
        throw;
    }
}

void Configuration::LoadImpl()
{
    auto data = toml::parse_file(path);
    iHotkey = data["iHotkey"sv].value_or(Default::iHotkey);
}

void Configuration::SaveImpl() const
{
    toml::table data;
    data.insert("iHotkey"sv, iHotkey);

    std::ofstream file{ path };
    file << data << std::endl;
}
