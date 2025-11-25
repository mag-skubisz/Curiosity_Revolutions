#!/usr/bin/env bash
set -euo pipefail

here="$(cd "$(dirname "$0")" && pwd)"
curiosity_bin="$here/../curiosity"

if [ ! -x "$curiosity_bin" ]; then
	echo "pas de executable"
	exit 1
fi

tests=(
	"simple.prg terrain_11x9.txt"
	"program_turn.prg terrain_blocked_5x5.txt"
	"program_collect.prg terrain_maze_7x7.txt"
)

for t in "${tests[@]}"; do
	set -- $t
	prog="$2"
	terrain="$1"
	echo "==== Running $prog on $terrain ===="
	"$curiosity_bin" "$here/$prog" "$here/$terrain" || echo "-> run returned non-zero exit"
	echo
done

