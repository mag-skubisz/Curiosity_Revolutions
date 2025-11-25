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
