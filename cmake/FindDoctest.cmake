include(FindPackageHandleStandardArgs)

find_path(DOCTEST_INCLUDE_DIR
          NAMES doctest/doctest.h
          PATHS ${PROJECT_SOURCE_DIR}/external
          NO_DEFAULT_PATH)
find_path(DOCTEST_INCLUDE_DIR NAMES doctest/doctest.h)

add_library(Doctest INTERFACE)
target_include_directories(Doctest INTERFACE $<BUILD_INTERFACE:${DOCTEST_INCLUDE_DIR}>)