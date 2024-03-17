#include "Configuration.h"

#include <toml++/toml.hpp>

namespace
{
    template <class T>
    std::vector<T> from_array(const toml::table& table, std::string_view key)
    {
        auto arr = table[key].as_array();
        if (!arr) {
            throw std::runtime_error(std::format("{} is not array.", key));
        }

        std::vector<T> vec;
        for (auto& ele : *arr) {
            vec.push_back(ele.value<T>().value());
        }
        return vec;
    }

    template <class T>
    toml::array to_array(const std::vector<T>& vec)
    {
        toml::array arr;
        for (auto& ele : vec) {
            arr.push_back(ele);
        }
        return arr;
    }
}

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
        auto msg = std::format("Failed to load configuration from \"{}\": {}", path, e.what());
        SKSE::stl::report_and_fail(msg);
    }
}

void Configuration::Save() const
{
    std::scoped_lock lock{ mutex };
    try {
        SaveImpl();
        SKSE::log::info("Saved configuration to \"{}\".", path);
    } catch (const std::exception& e) {
        auto msg = std::format("Failed to save configuration to \"{}\": {}", path, e.what());
        SKSE::stl::report_and_fail(msg);
    }
}

void Configuration::LoadImpl()
{
    auto data = toml::parse_file(path);

    iHotkey = data["iHotkey"sv].value_or(Default::iHotkey);

    slHUDNames = from_array<std::string>(data, "slHUDNames"sv);
    slMenuNames = from_array<std::string>(data, "slMenuNames"sv);
    slBannedMenuNames = from_array<std::string>(data, "slBannedMenuNames"sv);
}

void Configuration::SaveImpl() const
{
    toml::table data;

    data.insert("iHotkey"sv, iHotkey);

    data.insert("slHUDNames"sv, to_array(slHUDNames));
    data.insert("slMenuNames"sv, to_array(slMenuNames));
    data.insert("slBannedMenuNames"sv, to_array(slBannedMenuNames));

    std::ofstream file{ path };
    file << data << std::endl;
}
