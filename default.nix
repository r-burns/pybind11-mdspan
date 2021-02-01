with import <nixpkgs> {};
rec {
  pymdspan = python3.pkgs.callPackage ./pymdspan.nix {
    mdspan = callPackage ./mdspan.nix {};
  };
  tests = python3.pkgs.callPackage ./tests.nix {
    inherit pymdspan;
  };
}
