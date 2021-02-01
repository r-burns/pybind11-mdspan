{ lib, stdenv
, cmake 
, mdspan
, numpy
, pybind11
, pytest
, python3
}:
stdenv.mkDerivation rec {
  name = "pymdspan";

  src = lib.cleanSourceWith {
    filter = name: type: let
      basename = baseNameOf (toString name);
    in !(
      basename == "build" ||
      lib.hasSuffix ".nix" basename ||
      lib.hasSuffix ".py" basename
    );
    src = lib.cleanSource ./.;
  };

  CXXFLAGS = lib.concatStringsSep " " [
    "-Wall"
    "-Wextra"
    "-Wpedantic"
    "-Wno-unused-value" # coercing parameter packs
  ];

  nativeBuildInputs = [
    cmake
  ];
  buildInputs = [
    mdspan
    pybind11
  ];
  nativeInstallCheckInputs = [
    pytest
  ];
  installCheckInputs = [
    numpy
    pytest
  ];

  cmakeFlags = [
    "-DPYTHON_PKGDIR=${python3.sitePackages}"
  ];
}
