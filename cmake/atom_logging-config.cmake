include(CMakeFindDependencyMacro)

find_dependency(atom_core REQUIRED)

include("${CMAKE_CURRENT_LIST_DIR}/atom_logging-targets.cmake")
