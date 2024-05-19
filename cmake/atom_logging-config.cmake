include(CMakeFindDependencyMacro)

find_dependency(Catch2 REQUIRED)

include("${CMAKE_CURRENT_LIST_DIR}/atom_logging-targets.cmake")
