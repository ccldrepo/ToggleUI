cmake_minimum_required(VERSION 3.28)

project(
    ccld_ToggleUI
    VERSION 3.0.0
    DESCRIPTION "SKSE Plugin that allows users to toggle UI."
    LANGUAGES CXX
)

set(BUILD_SHARED_LIBS OFF)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION "$<$<CONFIG:Release>:ON>")

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.rc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/version.rc"
    @ONLY
)

include(cmake/headerlist.cmake)
include(cmake/sourcelist.cmake)

source_group(
    TREE
        ${CMAKE_CURRENT_SOURCE_DIR}
    FILES
        ${headers}
        ${sources}
)

# ---- Declare Dependencies ----

if(DEFINED ENV{VENDOR_GITHUB})
    cmake_path(SET VENDOR_GITHUB "$ENV{VENDOR_GITHUB}")
else()
    set(VENDOR_GITHUB "https://github.com")
endif()

include(FetchContent)
include(cmake/deps/fmt.cmake)
include(cmake/deps/rapidcsv.cmake)
include(cmake/deps/spdlog.cmake)
include(cmake/deps/tomlplusplus.cmake)
include(cmake/deps/CommonLibSSE-NG.cmake)

FetchContent_MakeAvailable(
    fmt
    rapidcsv
    spdlog
    tomlplusplus
    CommonLibSSE
)

# ---- Declare Targets ----

include("${commonlibsse_SOURCE_DIR}/cmake/CommonLibSSE.cmake")

add_commonlibsse_plugin(
    "${PROJECT_NAME}"
    SOURCES
        ${headers}
        ${sources}
        "${CMAKE_CURRENT_BINARY_DIR}/version.rc"
)

target_compile_definitions(
    ${PROJECT_NAME}
    PRIVATE
        _UNICODE
        NOMINMAX
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
