cmake_minimum_required(VERSION 3.5)
project(
    atom.logging
    VERSION 0.1.0
    LANGUAGES CXX)

include(FetchContent)
set(FETCHCONTENT_QUIET "OFF")

FetchContent_Declare(
    atom_core
    GIT_REPOSITORY "https://github.com/shifu-dev/atom.core.git"
    GIT_TAG "dev"
    GIT_SHALLOW true
    GIT_PROGRESS true)

FetchContent_Declare(
    catch2
    GIT_REPOSITORY "https://github.com/catchorg/Catch2.git"
    GIT_TAG "v3.5.1"
    GIT_SHALLOW true
    GIT_PROGRESS true)

FetchContent_MakeAvailable(atom_core catch2)

enable_testing()
set(CMAKE_CXX_STANDARD "23")
set(CMAKE_CXX_STANDARD_REQUIRED "TRUE")

# --------------------------------------------------------------------------------------------------
# atom.logging
# --------------------------------------------------------------------------------------------------

add_library(atom.logging STATIC)
target_sources(atom.logging PUBLIC FILE_SET CXX_MODULES FILES "source/atom.logging.cppm")
target_include_directories(atom.logging PRIVATE "include/")
target_link_libraries(atom.logging PRIVATE atom.core)

# --------------------------------------------------------------------------------------------------
# atom.logging Tests
# --------------------------------------------------------------------------------------------------

file(GLOB_RECURSE test_sources "tests/*.cpp")

add_executable(atom.logging.tests "${test_sources}")
target_link_libraries(atom.logging.tests PRIVATE atom.logging)
target_link_libraries(atom.logging.tests PRIVATE Catch2::Catch2WithMain)

add_test(atom.logging.tests atom.logging.tests)

# --------------------------------------------------------------------------------------------------
# atom.logging.sandbox
# --------------------------------------------------------------------------------------------------

add_executable(atom.logging.sandbox)
target_sources(atom.logging.sandbox PRIVATE FILE_SET CXX_MODULES FILES "sandbox/sandbox.cpp")

target_link_libraries(atom.logging.sandbox PRIVATE atom.logging)
