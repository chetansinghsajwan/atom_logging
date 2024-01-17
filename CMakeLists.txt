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

include("${atom-core_SOURCE_DIR}/CMake/enable-modules.cmake")

# --------------------------------------------------------------------------------------------------
# C++ Global config
# --------------------------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD "23")
set(CMAKE_CXX_STANDARD_REQUIRED "ON")
enable_testing()

# --------------------------------------------------------------------------------------------------
# Atom.Logging
# --------------------------------------------------------------------------------------------------

add_library(Atom.Logging STATIC)
target_sources(Atom.Logging PUBLIC
    FILE_SET modules TYPE CXX_MODULES FILES
    "Source/Private/Logging.cpp")
target_include_directories(Atom.Logging PRIVATE "Source/Private/")
target_include_directories(Atom.Logging PUBLIC "Source/Public/")

target_link_libraries(Atom.Logging PUBLIC Atom.Core)

# --------------------------------------------------------------------------------------------------
# Atom.Logging Tests
# --------------------------------------------------------------------------------------------------

file(GLOB_RECURSE TEST_SOURCES "Source/Tests/**.cpp")

add_executable(Atom.Logging.Tests ${TEST_SOURCES})
target_include_directories(Atom.Logging.Tests PUBLIC "Source/Tests/")

target_link_libraries(Atom.Logging.Tests PUBLIC Atom.Logging)
target_link_libraries(Atom.Logging.Tests PUBLIC Catch2::Catch2WithMain)

add_test(Atom.Logging.Tests Atom.Logging.Tests)
