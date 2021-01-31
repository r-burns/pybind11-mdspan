{ lib, stdenv
, cmake
, fetchFromGitHub
, fetchpatch
, gtest
}:
stdenv.mkDerivation rec {
  pname = "mdspan";
  version = "unstable";

  src = fetchFromGitHub {
    owner = "kokkos";
    repo = pname;
    rev = "371250b2341e6196b823337e4ea4d3474c879310";
    sha256 = "10341943dlmxb2j64wnbryaqmv6b1n1pd407wdzvxdvk3a0sv0kr";
  };
  patches = [
    (fetchpatch {
      url = "https://github.com/kokkos/mdspan/commit/626fb681b34e62d42ac0d66dd914343c6711c1e0.patch";
      sha256 = "0mmfdh8lcfdqmw27xl897vzzs3hl4i0d62xi3v9mfzp731cpkc64";
    })
  ];

  nativeBuildInputs = [
    cmake
  ];

  buildInputs = lib.optionals doCheck [
    gtest
  ];

  # TODO stdenv.buildPlatform == stdenv.hostPlatform;
  # gtest is fetched unconditionally
  doCheck = false;

  cmakeFlags = [
    "-DMDSPAN_ENABLE_TESTS=${lib.boolToString doCheck}"
  ];

  meta = with lib; {
    description = ''
      Production-quality reference implementation of mdspan
    '';
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
