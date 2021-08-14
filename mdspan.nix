{ lib, stdenv
, cmake
, fetchFromGitHub
, fetchpatch
, gtest
}:
stdenv.mkDerivation rec {
  pname = "mdspan";
  version = "0.1.0";

  src = fetchFromGitHub {
    owner = "kokkos";
    repo = pname;
    rev = "mdspan-${version}";
    sha256 = "0hyfgrsqrj7nhs148klbhg5ckm0npwnhhbqal98jbkphiz6xnb55";
  };

  patches = [
    # Remove exported target's interface C++ standard
    (fetchpatch {
      url = "https://github.com/kokkos/mdspan/commit/c9058a9f9d8fe9a1f69d20d9ba7ad23d820b950c.patch";
      sha256 = "1zjk14rasa2856dfvyk8hma9annhjw944m691hp87kl0ykjrzkjw";
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
