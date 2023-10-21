cmake_minimum_required(VERSION 3.5)
project(Atom.Logging VERSION 0.1.0 LANGUAGES CXX C)

include(CMake/CPM.cmake)
set(CPM_SOURCE_CACHE "${CMAKE_SOURCE_DIR}/Build/CPMCache")

CPMAddPackage("gh:shifu-dev/Atom.Core#dev")
CPMAddPackage("gh:catchorg/Catch2@3.4.0")

# --------------------------------------------------------------------------------------------------
# C++ Global config
# --------------------------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD "23")
set(CMAKE_CXX_STANDARD_REQUIRED "ON")
enable_testing()

# --------------------------------------------------------------------------------------------------
# Atom.Logging
# --------------------------------------------------------------------------------------------------

file(GLOB_RECURSE SOURCES "Source/Private/**.cpp"
    "Source/Private/**.h" "Source/Public/**.h")

add_library(Atom.Logging STATIC ${SOURCES})
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