cmake_minimum_required(VERSION 3.28 FATAL_ERROR)
project(
    atom.logging
    VERSION 0.1.0
    LANGUAGES CXX)

find_package(atom_core REQUIRED)
find_package(Catch2 REQUIRED)

# --------------------------------------------------------------------------------------------------
# atom.logging
# --------------------------------------------------------------------------------------------------

add_library(atom.logging STATIC)

file(GLOB_RECURSE modules "sources/*.cppm")
target_sources(atom.logging PUBLIC FILE_SET CXX_MODULES FILES ${modules})
target_link_libraries(atom.logging PUBLIC atom::atom.core)
target_compile_features(atom.logging PUBLIC "cxx_std_23")

# --------------------------------------------------------------------------------------------------
# atom.logging Tests
# --------------------------------------------------------------------------------------------------

add_executable(atom.logging.tests)

file(GLOB_RECURSE test_sources "tests/*.cpp")
target_sources(atom.logging.tests PRIVATE ${test_sources})

target_link_libraries(atom.logging.tests PRIVATE atom.logging)
target_link_libraries(atom.logging.tests PRIVATE Catch2::Catch2WithMain)

add_test(atom.logging.tests atom.logging.tests)

# --------------------------------------------------------------------------------------------------
# atom.logging.sandbox
# --------------------------------------------------------------------------------------------------

add_executable(atom.logging.sandbox)
target_sources(atom.logging.sandbox PRIVATE FILE_SET CXX_MODULES FILES "sandbox/sandbox.cpp")
target_link_libraries(atom.logging.sandbox PRIVATE atom.logging)

# --------------------------------------------------------------------------------------------------
# install phase
# --------------------------------------------------------------------------------------------------

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "atom_logging-config-version.cmake"
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY AnyNewerVersion)

install(
    TARGETS atom.logging
    EXPORT atom_logging-targets
    FILE_SET CXX_MODULES DESTINATION "lib")

install(
    EXPORT atom_logging-targets
    FILE "atom_logging-targets.cmake" DESTINATION "cmake/"
    NAMESPACE "atom::")

install(FILES
    "cmake/atom_logging-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/atom_logging-config-version.cmake"
    DESTINATION "cmake/")
