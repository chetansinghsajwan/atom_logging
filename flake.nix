{
    description = "atom_logging";

    inputs = {
        nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";

        atom_doc = {
            url = "github:shifu-dev/atom-doc";
            inputs.nixpkgs.follows = "nixpkgs";
        };

        atom_core = {
            url = "github:shifu-dev/atom_core";
            inputs.nixpkgs.follows = "nixpkgs";
        };
    };

    outputs = inputs:
    let
        system = "x86_64-linux";
        pkgs = inputs.nixpkgs.legacyPackages.${system};
        lib = pkgs.lib;
        stdenv = pkgs.llvmPackages_18.libcxxStdenv;
        atom_doc_pkg = inputs.atom_doc.packages.${system}.default;
        atom_core_env = inputs.atom_core.env.${system}.default;
        atom_core_pkg = inputs.atom_core.packages.${system}.default;
    in rec
    {
        env.${system}.default = rec {

            name = "atom_logging";
            src = ./.;

            propagatedBuildInputs = with pkgs; [
                atom_core_pkg
            ];

            nativeBuildInputs = with pkgs; [
                atom_doc_pkg
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
                cmake --build build --target atom_logging;
            '';

            installPhase = ''
                cmake --install build --prefix $out;
            '';

            clang_scan_deps_include_paths = [
                "${atom_core_pkg}/include"
            ];

            envVars = {

                CXXFLAGS = lib.strings.concatMapStrings (v: " -I " + v)
                    (atom_core_env.clang_scan_deps_include_paths ++ clang_scan_deps_include_paths);

                CMAKE_GENERATOR = "Ninja";
                CMAKE_BUILD_TYPE = "Debug";
                CMAKE_EXPORT_COMPILE_COMMANDS = "true";
                ATOM_DOC_DOXYFILE_DIR = atom_doc_pkg;
            };
        };

        devShells.${system}.default = with env.${system}.default; stdenv.mkDerivation ({
            inherit name;
            inherit src;
            inherit propagatedBuildInputs;
            inherit nativeBuildInputs;
        } // envVars);

        packages.${system}.default = with env.${system}.default; stdenv.mkDerivation ({
            inherit name;
            inherit src;
            inherit propagatedBuildInputs;
            inherit nativeBuildInputs;
            inherit configurePhase;
            inherit buildPhase;
            inherit installPhase;
        } // envVars);
    };
}
