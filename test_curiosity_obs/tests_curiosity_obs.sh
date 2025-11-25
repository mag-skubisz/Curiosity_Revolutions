#!/bin/bash

BIN="../curiosity-obs"
if [ ! -x "$BIN" ]; then
  echo "error"
  exit 1
fi

run_one() {
  terrain=$1
  prog=$2
  echo "----"
  echo "Terrain: $terrain   Programme: $prog"
  $BIN "$terrain" "$prog"
}

# 1) correct program, accepted by observer
run_one "terrain_free.txt" "prog_accept_ok.prg"

# 2) incorrect program, rejected by observer
run_one "terrain_free.txt" "prog_reject_incorrect.prg"

# 3) correct program, rejected by observer (rotation between M and A)
run_one "terrain_free.txt" "prog_correct_reject.prg"

# 4) incorrect program (advance into rock), accepted by observer
run_one "terrain_blocked.txt" "prog_incorrect_accept.prg"

echo "----Fini.-----" 

#!/bin/bash

BIN="../curiosity-obs"

# Sprawdzenie, czy program istnieje i jest wykonywalny
[ -x "$BIN" ] || { echo "error"; exit 1; }

run() {
  echo "----"
  echo "Terrain: $1   Programme: $2"
  "$BIN" "$1" "$2"
}

run "terrain_free.txt"      "prog_accept_ok.prg"
run "terrain_free.txt"      "prog_reject_incorrect.prg"
run "terrain_free.txt"      "prog_correct_reject.prg"
run "terrain_blocked.txt"   "prog_incorrect_accept.prg"

echo "----Fini.-----"
