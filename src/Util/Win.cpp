#include "Win.h"

#include <Windows.h>

using RtlGetVersionFuncPtr = NTSTATUS(WINAPI*)(PRTL_OSVERSIONINFOEXW);

std::optional<OsVersion> GetOsVersion() noexcept
{
    HMODULE hNtdll = GetModuleHandleW(L"ntdll.dll");
    if (hNtdll == nullptr) {
        return {};
    }

    auto RtlGetVersion = reinterpret_cast<RtlGetVersionFuncPtr>(GetProcAddress(hNtdll, "RtlGetVersion"));
    if (RtlGetVersion == nullptr) {
        return {};
    }

    RTL_OSVERSIONINFOEXW ovi;
    ovi.dwOSVersionInfoSize = sizeof(ovi);
    if (RtlGetVersion(&ovi) != 0) {
        return {};
    }
    return OsVersion{ ovi.dwMajorVersion, ovi.dwMinorVersion, ovi.dwBuildNumber };
}
