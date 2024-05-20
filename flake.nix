{
    description = "atom.logging";

    inputs = {
        nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";

        atom_core.url = "github:shifu-dev/atom.core";
        atom_core.inputs.nixpkgs.follows = "nixpkgs";
    };

    outputs = inputs:
    let
        system = "x86_64-linux";
        pkgs = inputs.nixpkgs.legacyPackages.${system};
        lib = pkgs.lib;
        stdenv = pkgs.llvmPackages_18.libcxxStdenv;
        atom_core_pkg = inputs.atom_core.packages.${system}.default;

        clang_scan_deps_include_paths = inputs.atom_core.clang_scan_deps_include_paths +
            " -I ${atom_core_pkg}/include";

        derivation = stdenv.mkDerivation {

            name = "atom.logging";

            src = ./.;

            propagatedBuildInputs = with pkgs; [
                atom_core_pkg
            ];

            nativeBuildInputs = with pkgs; [
                catch2_3

                cmake
                cmake-format
                ninja
                git
            ];

            configurePhase = ''
                cmake -S . -B build \
                    -D atom_core_DIR:PATH=${atom_core_pkg};
            '';

            buildPhase = ''
                cmake --build build --target atom.logging;
            '';

            installPhase = ''
                cmake --install build --prefix $out;
            '';

            CXXFLAGS = clang_scan_deps_include_paths;
            CMAKE_GENERATOR = "Ninja";
            CMAKE_BUILD_TYPE = "Debug";
            CMAKE_EXPORT_COMPILE_COMMANDS = "true";
        };
    in
    {
        inherit clang_scan_deps_include_paths;

        devShells.${system}.default = derivation;

        packages.${system}.default = derivation;
    };
}
