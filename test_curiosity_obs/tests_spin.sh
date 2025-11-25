#!/bin/bash

BIN="../curiosity-obs"

# Sprawdzenie, czy program istnieje i jest wykonywalny
[ -x "$BIN" ] || { echo "error"; exit 1; }

run() {
  echo "----"
  echo "Terrain: $1   Programme: $2"
  "$BIN" "$1" "$2"
}

# 1) programme correct, accepté par l'observateur (pas plus de 3 rotations consécutives)
run "terrain_free.txt" "prog_spin_accept_ok.prg"
# 2) programme incorrect (violation spin), rejeté par l'observateur
run "terrain_free.txt" "prog_spin_reject_incorrect.prg"
# 3) programme correct (logiquement), mais rejeté par l'observateur (4 rotations consécutives)
run "terrain_free.txt" "prog_spin_correct_reject.prg"
# 4) programme incorrect (crash) mais accepté par l'observateur (aucune violation spin)
run "terrain_blocked.txt" "prog_spin_incorrect_accept.prg"

echo "----Fini.-----"