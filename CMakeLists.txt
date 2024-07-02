cmake_minimum_required(VERSION 3.28 FATAL_ERROR)
project("atom_logging" VERSION "0.1.0")
enable_language(CXX)

# --------------------------------------------------------------------------------------------------
# options
# --------------------------------------------------------------------------------------------------

option(atom_logging_build_tests "Enable this to build tests." OFF)

# --------------------------------------------------------------------------------------------------
# atom_logging
# --------------------------------------------------------------------------------------------------

find_package("atom_core" REQUIRED)

add_library(atom_logging STATIC)

file(GLOB_RECURSE atom_logging_modules "sources/*.cppm")
target_sources(atom_logging PUBLIC FILE_SET CXX_MODULES FILES "${atom_logging_modules}")
target_link_libraries(atom_logging PUBLIC "atom_core")
target_compile_features(atom_logging PUBLIC "cxx_std_20")

# --------------------------------------------------------------------------------------------------
# tests
# --------------------------------------------------------------------------------------------------

if (atom_logging_build_tests)
    find_package(Catch2 REQUIRED)

    add_executable(atom_logging_tests)

    file(GLOB_RECURSE atom_logging_test_sources "tests/*.cpp")
    target_sources(atom_logging_tests PRIVATE "${atom_logging_test_sources}")

    target_link_libraries(atom_logging_tests PRIVATE "atom_logging")
    target_link_libraries(atom_logging_tests PRIVATE "Catch2::Catch2WithMain")

    add_test(atom_logging_tests atom_logging_tests)
endif()

# --------------------------------------------------------------------------------------------------
# sandbox
# --------------------------------------------------------------------------------------------------

add_executable(atom_logging_sandbox)
target_sources(atom_logging_sandbox PRIVATE FILE_SET CXX_MODULES FILES "sandbox/sandbox.cpp")
target_link_libraries(atom_logging_sandbox PRIVATE atom_core atom_logging)

# --------------------------------------------------------------------------------------------------
# install
# --------------------------------------------------------------------------------------------------

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "atom_logging-config-version.cmake"
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY AnyNewerVersion)

install(
    TARGETS atom_logging
    EXPORT atom_logging-targets
    FILE_SET CXX_MODULES DESTINATION "lib")

install(
    EXPORT atom_logging-targets
    FILE "atom_logging-targets.cmake" DESTINATION "cmake/")

install(FILES
    "cmake/atom_logging-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/atom_logging-config-version.cmake"
    DESTINATION "cmake/")
