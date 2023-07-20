{ lib, stdenv
, cmake
, fetchFromGitHub
, fetchpatch
, gtest
}:
stdenv.mkDerivation rec {
  pname = "mdspan";
  version = "0.6.0";

  src = fetchFromGitHub {
    owner = "kokkos";
    repo = pname;
    rev = "mdspan-${version}";
    sha256 = "17zmjid1vjvpmvgd1k023ljk8yygqw18xilx78b7pxg7xws3w0bg";
  };

  nativeBuildInputs = [
    cmake
  ];

  buildInputs = lib.optionals doCheck [
    gtest
  ];

  doCheck = true;

  cmakeFlags = [
    "-DMDSPAN_ENABLE_TESTS=${lib.boolToString doCheck}"
    "-DMDSPAN_USE_SYSTEM_GTEST=ON"
  ];

  meta = with lib; {
    description = "Production-quality reference implementation of mdspan";
    longDescription = ''
      The ISO-C++ proposal P0009 will add support for non-owning
      multi-dimensional array references to the C++ standard library.
      This repository aims to provide a production-quality implementation
      of the proposal as written (with a few caveats) in preparation for
      the addition of mdspan to the standard.
    '';
    maintainers = with maintainers; [ r-burns ];
    license = licenses.bsd3;
  };
}
