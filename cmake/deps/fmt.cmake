# https://github.com/fmtlib/fmt
set(FMT_DOC OFF CACHE INTERNAL "")
set(FMT_INSTALL OFF CACHE INTERNAL "")
set(FMT_TEST OFF CACHE INTERNAL "")
set(FMT_OS ON CACHE INTERNAL "")
FetchContent_Declare(
    fmt
    URL      "${VENDOR_GITHUB}/fmtlib/fmt/archive/refs/tags/10.2.1.tar.gz"
    URL_HASH SHA256=1250e4cc58bf06ee631567523f48848dc4596133e163f02615c97f78bab6c811
    OVERRIDE_FIND_PACKAGE
)
