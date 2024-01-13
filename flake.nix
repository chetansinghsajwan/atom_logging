{
    description = "atom-engine";

    inputs = {
        nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    };

    outputs = { self, nixpkgs }:
    let
        system = "x86_64-linux";
        pkgs = nixpkgs.legacyPackages.${system};
    in
    {
        devShells.${system}.default = pkgs.mkShell {
            
            packages = with pkgs; [
                cmake
                gnumake
                ninja
		        gcc13
		        clang_17
		        clang-tools_17
            ];
        };
    };
}
