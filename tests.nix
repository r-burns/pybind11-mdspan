{ lib, stdenv
, numpy
, pymdspan
, pytestCheckHook
}:
stdenv.mkDerivation rec {
  name = "pymdspan-pytests";

  src = lib.cleanSourceWith {
    filter = name: type: let
      basename = baseNameOf (toString name);
    in lib.hasSuffix ".py" basename;
    src = lib.cleanSource ./.;
  };

  nativeBuildInputs = [
    numpy
    pymdspan
    pytestCheckHook
  ];

  installPhase = "touch $out";
}
