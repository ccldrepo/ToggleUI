#pragma once

#include <algorithm>
#include <array>
#include <atomic>
#include <bit>
#include <cctype>
#include <chrono>
#include <climits>
#include <compare>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <exception>
#include <filesystem>
#include <format>
#include <fstream>
#include <functional>
#include <initializer_list>
#include <ios>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <ranges>
#include <set>
#include <shared_mutex>
#include <span>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <system_error>
#include <thread>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include <RE/Skyrim.h>
#include <REL/Relocation.h>
#include <REX/PS4.h>
#include <REX/W32.h>
#include <SKSE/SKSE.h>

#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ranges.h>

using namespace std::literals::string_view_literals;

namespace SKSE::stl
{
    template <class T>
    inline void write_thunk_call(REL::Relocation<std::uintptr_t> a_src)
    {
        SKSE::AllocTrampoline(14);
        T::func = a_src.write_call<5>(T::tunk);
    }
}
