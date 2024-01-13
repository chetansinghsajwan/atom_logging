{
    description = "atom-engine";

    inputs = {
        nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    };

    outputs = { self, nixpkgs }:
    let
        system = "x86_64-linux";
        pkgs = nixpkgs.legacyPackages.${system};

        atom-core = pkgs.fetchFromGitHub {
            owner = "shifu-dev";
            repo = "Atom.Core";
            rev = "dev";
            sha256 = "sha256-XuGBLoVDY6NxVcTD0cYzX71bMV2opQ77FRlvdtgHAHE";
        };
    in
    {
        devShells.${system}.default = pkgs.mkShell {
            
            packages = with pkgs; [
                cmake
                gnumake
                ninja
		catch2
            ];

            env = {
                "ATOM_CORE" = atom-core;
            };
        };
    };
}
