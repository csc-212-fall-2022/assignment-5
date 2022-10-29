cmake_minimum_required(VERSION 3.14)

project("some assignment")

set(CMAKE_CXX_STANDARD 17)

# Setting -ggdb3 gets line numbers in Valgrind output
# -Og is the recommended level of optimization from the GCC docs for "the standard edit-compile-debug" cycle
# https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb3 -Og")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake) # make sure we know where to find the doctest header

# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/assignment1)
find_package(Doctest REQUIRED)

include(cmake/ClangFormat.cmake)

set(CMAKE_MEMORYCHECK_COMMAND valgrind)

# Use include(CTest) rather than enable_testing() to get Valgrind working
include(CTest)

find_program(CLANG_TIDY_EXE NAME clang-tidy REQUIRED)
set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}" "-checks=-*,modernize-*,bugprone-*")
