with import <nixpkgs> {};
python3.pkgs.callPackage ./pymdspan.nix {
  mdspan = callPackage ./mdspan.nix {};
}
