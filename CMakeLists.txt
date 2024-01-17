cmake_minimum_required(VERSION 3.5)
project(Atom.Logging VERSION 0.1.0 LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
    atom-core
    GIT_REPOSITORY "https://github.com/shifu-dev/Atom.Core.git"
    GIT_TAG        "dev"
    GIT_SHALLOW    true
)

FetchContent_Declare(
    catch2
    GIT_REPOSITORY "https://github.com/catchorg/Catch2.git"
    GIT_TAG        "v3.5.1"
    GIT_SHALLOW    true
)

FetchContent_MakeAvailable(atom-core catch2)
FetchContent_GetProperties(atom-core)

include("${atom-core_SOURCE_DIR}/cmake/enable_modules.cmake")

# --------------------------------------------------------------------------------------------------
# Atom.Logging
# --------------------------------------------------------------------------------------------------

add_library(atom.logging STATIC)
target_sources(atom.logging PUBLIC
    FILE_SET modules TYPE CXX_MODULES FILES
    "src/logging.cpp")
target_include_directories(atom.logging PRIVATE "src/")

target_link_libraries(atom.logging PUBLIC atom.core)

# --------------------------------------------------------------------------------------------------
# Atom.Logging Tests
# --------------------------------------------------------------------------------------------------

file(GLOB_RECURSE test_sources "tests/*.cpp")

add_executable(atom.logging.tests "${test_sources}")
target_include_directories(atom.logging.tests PUBLIC "tests/")

target_link_libraries(atom.logging.tests PUBLIC atom.logging)
target_link_libraries(atom.logging.tests PUBLIC Catch2::Catch2WithMain)

add_test(atom.logging.tests atom.logging.tests)
