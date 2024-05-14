{
    description = "atom.logging";

    inputs = {
        nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    };

    outputs = { self, nixpkgs }:
    let
        system = "x86_64-linux";
        pkgs = nixpkgs.legacyPackages.${system};
        stdenv = pkgs.llvmPackages_18.libcxxStdenv;
    in
    {
        devShells.${system}.default = stdenv.mkDerivation {
            name = "atom.logging";
            buildInputs = with pkgs; [
                cmake
                cmake-format
                ninja
                git
            ];
        };
    };
}
