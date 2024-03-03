# https://github.com/CharmedBaryon/CommonLibSSE-NG
set(COMMONLIBSSE_INSTALL OFF CACHE INTERNAL "")
set(BUILD_TESTS OFF CACHE INTERNAL "")
FetchContent_Declare(
    CommonLibSSE
    GIT_REPOSITORY https://github.com/ccldrepo/CommonLibSSE-NG
    GIT_TAG        8690bf0e056435846cf154e3d23a29e1346e48b8
    OVERRIDE_FIND_PACKAGE
)
