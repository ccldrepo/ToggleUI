cmake_minimum_required(VERSION 3.28)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(
    ccld_ToggleUI
    VERSION 1.0.0
    DESCRIPTION "SKSE Plugin that allows users to toggle UI."
    LANGUAGES CXX
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.rc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/version.rc"
    @ONLY
)

include(cmake/headerlist.cmake)
include(cmake/sourcelist.cmake)

# ---- Declare Dependencies ----

if(DEFINED ENV{VENDOR_GITHUB})
    cmake_path(SET VENDOR_GITHUB "$ENV{VENDOR_GITHUB}")
else()
    set(VENDOR_GITHUB "https://github.com")
endif()

include(FetchContent)
include(cmake/deps/tomlplusplus.cmake)

find_package(CommonLibSSE CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(tomlplusplus CONFIG REQUIRED)

# ---- Declare Targets ----

add_commonlibsse_plugin(
    "${PROJECT_NAME}"
    SOURCES
        ${headers}
        ${sources}
        "${CMAKE_CURRENT_BINARY_DIR}/version.rc"
)

target_compile_features(
    "${PROJECT_NAME}"
    PRIVATE
        cxx_std_23
)

target_include_directories(
    "${PROJECT_NAME}"
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_link_libraries(
    "${PROJECT_NAME}"
    PRIVATE
        fmt::fmt
        spdlog::spdlog
        tomlplusplus::tomlplusplus
)

target_precompile_headers(
    "${PROJECT_NAME}"
    PRIVATE
        src/PCH.h
)
