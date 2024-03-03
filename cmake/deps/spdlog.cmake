# https://github.com/gabime/spdlog
set(SPDLOG_BUILD_EXAMPLE OFF CACHE INTERNAL "")
set(SPDLOG_BUILD_TESTS OFF CACHE INTERNAL "")
set(SPDLOG_INSTALL OFF CACHE INTERNAL "")
set(SPDLOG_FMT_EXTERNAL ON CACHE INTERNAL "")
FetchContent_Declare(
    spdlog
    URL      "${VENDOR_GITHUB}/gabime/spdlog/archive/refs/tags/v1.13.0.tar.gz"
    URL_HASH SHA256=534f2ee1a4dcbeb22249856edfb2be76a1cf4f708a20b0ac2ed090ee24cfdbc9
    OVERRIDE_FIND_PACKAGE
)
