cmake_minimum_required(VERSION 3.14)

project("Assignment 5")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)

# Setting -ggdb3 gets line numbers in Valgrind output
# -Og is the recommended level of optimization from the GCC docs for "the standard edit-compile-debug" cycle
# https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb3 -Og")

# Attempt to fix the icu issues on OS X. If this doesn't work, let me know
if (APPLE OR (CMAKE_SYSTEM_NAME EQUAL "Darwin"))
    # prefer homebrew-install icu4c since the default seems not to be working
    set(CMAKE_PREFIX_PATH "/opt/hombrew/opt/icu4c")

    list(TRANSFORM CMAKE_PREFIX_PATH APPEND "/include"
            OUTPUT_VARIABLE CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES)
    set(CMAKE_C_STANDARD_INCLUDE_DIRECTORIES "${CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES}")
else()
    # OS X folks don't have clang tidy
    find_program(CLANG_TIDY_EXE NAME clang-tidy REQUIRED)
    set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}" "-checks=-*,modernize-*,bugprone-*")
endif ()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake) # make sure we know where to find the doctest header

find_package(Doctest REQUIRED)

include(cmake/ClangFormat.cmake)

set(CMAKE_MEMORYCHECK_COMMAND valgrind)

# Use include(CTest) rather than enable_testing() to get Valgrind working
include(CTest)
include(FetchContent)


set(Boost_DEBUG ON)
find_package(Boost COMPONENTS program_options)
if (NOT Boost_FOUND)
    message(STATUS "Boost not found. Downloading.")
    FetchContent_Declare(boost
            GIT_REPOSITORY https://github.com/boostorg/boost.git
            GIT_TAG boost-1.80.0
            GIT_SHALLOW true
            )

    FetchContent_MakeAvailable(boost)
else ()
    include_directories(${Boost_INCLUDE_DIRS})
endif ()

find_package(ICU COMPONENTS uc REQUIRED)

add_library(Ballot assignment5/ballot.hpp assignment5/ballot.cpp)
target_link_libraries(Ballot Boost::boost ICU::uc)

add_executable(BallotTest assignment5/ballot.test.cpp)
target_link_libraries(BallotTest Ballot Doctest)

add_test(BallotTest BallotTest)

add_library(ElectionLib assignment5/ElectionQueue.cpp assignment5/ElectionQueue.hpp assignment5/CandidateVotes.cpp assignment5/CandidateVotes.hpp assignment5/Election.hpp assignment5/Election.cpp)
target_link_libraries(ElectionLib Ballot)
add_executable(Election assignment5/main.cpp)
target_link_libraries(Election ElectionLib Boost::program_options)

add_executable(ElectionTest assignment5/Election.test.cpp)
target_link_libraries(ElectionTest ElectionLib Doctest)

add_test(ElectionTest ElectionTest)
