{ lib, stdenv
, cmake 
, mdspan
, pybind11
, python3
}:
stdenv.mkDerivation rec {
  name = "pymdspan";

  src = lib.cleanSourceWith {
    filter = name: type: let
      basename = baseNameOf (toString name);
    in !(
      lib.hasSuffix ".nix" basename
    );
    src = lib.cleanSource ./.;
  };

  nativeBuildInputs = [
    cmake
  ];
  buildInputs = [
    mdspan
    pybind11
  ];

  cmakeFlags = [
    "-DPYTHON_PKGDIR=${python3.sitePackages}"
  ];
}
