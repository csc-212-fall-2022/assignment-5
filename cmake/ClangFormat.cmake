find_program(CLANG_FORMAT "clang-format")
if(NOT CLANG_FORMAT)
    message(FATAL_ERROR "clang-format not found on your \$\{PATH\}")
endif()

SET(EXCLUDE_PATTERN "${CMAKE_CURRENT_SOURCE_DIR}/(external|build|cmake-build-debug).*")

# See https://arcanis.me/en/2015/10/17/cppcheck-and-clang-format
file(GLOB_RECURSE ALL_SOURCE_FILES *.cpp *.hpp *.h)
foreach(SOURCE_FILE ${ALL_SOURCE_FILES})
    string(REGEX MATCH ${EXCLUDE_PATTERN} TO_EXCLUDE ${SOURCE_FILE})
    if(NOT ${TO_EXCLUDE} STREQUAL "")
        message(STATUS "Excluding" ${SOURCE_FILE})
        list(REMOVE_ITEM ALL_SOURCE_FILES ${SOURCE_FILE})
    endif()
endforeach()

if(NOT ALL_SOURCE_FILES)
    message(FATAL_ERROR "No files found for format!")
endif()

add_custom_target(
        format_check
        COMMENT "Checking formatting with clang-format"
        COMMAND ${CLANG_FORMAT} -Werror --dry-run -ferror-limit=1 --verbose -style=llvm ${ALL_SOURCE_FILES}
)

add_custom_target(
        format_all
        COMMENT "Formatting with clang-format"
        COMMAND ${CLANG_FORMAT} -i --verbose -style=llvm ${ALL_SOURCE_FILES}
)