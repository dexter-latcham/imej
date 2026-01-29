{
  description = "A very basic flake";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }: 
  flake-utils.lib.eachDefaultSystem (system:
    let
      pkgs = import nixpkgs {
        inherit system;
      };
    in
    {
    devShells.default = pkgs.mkShell {
      packages = [
        pkgs.xorg.libX11
      ];
      shellHook = ''
        echo "in nix shell"
      '';
    };
  });
}
