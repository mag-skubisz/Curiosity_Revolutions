# Curiosity Revolution

## Overview
This repository contains exercises and tests for the *Curiosity* project
(an interpreter / simulation of a robot on a terrain). Source modules (in C)
include: `environnement`, `interprete`, `programme`, `robot`, `terrain`,
`type_pile`.

# TP5

## Ex.1 and 2
Implementation and testing of `terrain.c` and `test_terrain.c`.

## Ex.3
Implementation and testing of `robot.c`.

## Ex.4
Implementation and testing of `robot_terrain`.
In the folders `Terrains_Corrects` and `Terrains_Incorrects` you can find
example terrains for tests.

To run the terrain test program:
```bash
make robot_terrain
./robot_terrain <terrain_file>
```

# TP6

## Ex.1 — Unit tests (script)
Basic tests for several terrains and programs are in the `test_curiosity`
folder. To run all tests in that folder (from the project root):

```bash
./test_curiosity/tests_curiosity.sh
```

This script runs the `curiosity` executable on multiple `(terrain, program)`
pairs and prints results.

## Ex.2 and 3 — Functional tests for `curiosity-test`
I added many test files to cover the possible terminal behaviors of the robot.
Each `test_*.txt` has the following format:

```
<terrain_file_name>
<program_file_name>
<max_steps>
<expected_event>          # one character: N, F, S, O, or P
[<x> <y>]                 # optional: if event = N or F, expected coordinates
[<orientation>]           # optional: if event = N or F, expected orientation
```

Meanings for expected events:
- `N` — normal position (robot is inside terrain and OK),
- `F` — program finished normally (stop),
- `S` — robot left the terrain (exit),
- `O` — robot hit an obstacle (crash),
- `P` — robot fell into water (plouf).

Example single test run (from project root):

```bash
./curiosity-test test_plouf.txt
```

## Complete list of tests
Below is the full list of `test_*.txt` files in the repository with a short
description of what they check:

- `test_plouf.txt` → `terrain_plouf.txt` + `prog_plouf.prg`: checks water fall
  detection (`P`).
- `test_crash.txt` → `terrain_crash.txt` + `prog_crash.prg`: checks rock crash
  detection (`O`).
- `test_sortie.txt` → `terrain_sortie.txt` + `prog_sortie.prg`: checks leaving
  the terrain (`S`).
- `test_arret.txt` → `terrain_arret.txt` + `prog_arret.prg`: checks normal stop
  (`F`) and final position/orientation.
- `test_add.txt` → `terrain_add.txt` + `prog_add.txt`: tests `Add` and arithmetic
  evaluation.
- `test_sub.txt` → `terrain_sub.txt` + `prog_sub.txt`: tests `Sub` operand
  ordering and its effect.
- `test_mult.txt` → `terrain_mult.txt` + `prog_mult.txt`: tests `Mult`.
- `test_div.txt` → `terrain_div.txt` + `prog_div.txt`: tests `Div` and error
  handling (divide by zero).
- `test_clone.txt` → `terrain_clone.txt` + `prog_clone.txt`: tests `Clone`.
- `test_enchanger.txt` → `terrain_enchanger.txt` + `prog_enchanger.txt`: tests
  swap operation (`Echange`).
- `test_rotation.txt` → `terrain_rotation.txt` + `prog_rotation.txt`: tests
  `Rotation` (cyclic shift of the top n stack elements). This test was tuned
  to cause a plouf (`P`) to validate rotation effects on loop values.
- `test_boucle.txt` → `terrain_boucle.txt` + `prog_boucle.txt`: tests `Boucle`
  and loop counter handling (expected `F` with final position).
- `test_condbloc.txt` → `terrain_condbloc.txt` + `prog_condbloc.txt`: tests
  conditional block execution (`CondExec`) and block address binding.
- `test_mesure.txt` → `terrain_mesure.txt` + `prog_mesure.prg`: tests the
  `Mesure` instruction and its return value on the stack (special case: param
  `8`).
- `test_ignore.txt` → `terrain_ignore.txt` + `prog_ignore.txt`: tests the
  `Ignore` instruction (popping a value) and usage.

Note: every `test_*.txt` follows the format described above.

To run a single test (from project root):

```bash
./curiosity-test test_rotation.txt
```

To run all the provided tests for `curiosity`:

```bash
./curiosity-test.sh
```

## Ex.4 — Run all interpreters tests

To run all tests for all interpreters, run:
```bash
run_all_curiosity_tests.sh
```

## Interpreters — quick summary

- `interprete0.c` — INCORRECT: bug in `Clone` (duplicates value twice).
- `interprete1.c` — OK: reference implementation.
- `interprete2.c` — INCORRECT: `Echange` does not reverse order (push order
  wrong).
- `interprete3.c` — INCORRECT: `Mesure` uses `param % 8` (loses case `8`).
- `interprete4.c` — INCORRECT: `CondExec` overwrites the 'else' block address.
- `interprete5.c` — INCORRECT: `DebutBloc` pushes return address twice.
- `interprete6.c` — INCORRECT: return codes for `PLOUF` and `CRASH` are
  swapped.
- `interprete7.c` — OK.
- `interprete8.c` — INCORRECT: `Boucle` uses `n >= 0` instead of `n > 0`.
- `interprete9.c` — INCORRECT: `Sub` does `a - b` instead of `b - a`.

Summary: only `interprete1.c` and `interprete7.c` are correct; the others
contain single-line bugs that are easy to fix.

## Ex.5 — Interpreter robustness tests

1) Errors detected when reading a program (function `lire_programme` in
   `programme.c`):

- `ERREUR_FICHIER_PROGRAMME` — cannot open the program file.
- `ERREUR_BLOC_NON_FERME` — end of file reached while `{` remain unmatched.
- `ERREUR_FERMETURE_BLOC_EXCEDENTAIRE` — a `}` without a matching `{`.
- `ERREUR_COMMANDE_INCORRECTE` — unrecognized character/command.

Example: `prog_read_error.prg` (contents: `@`) → expected
`ERREUR_COMMANDE_INCORRECTE`.

2) Errors detected at execution (function `exec_pas` in `interprete.c`):

- `ERREUR_PILE_VIDE` — an operation required stack elements but stack was
  empty.
- `ERREUR_ADRESSAGE` — program counter (pc) became invalid.
- `ERREUR_DIVISION_PAR_ZERO` — attempted division by zero.

Example: `prog_divzero.prg` (contents: `5 0 /`) → `ERREUR_DIVISION_PAR_ZERO`.

## Ex.6 — Robustness tests (bonus)

In `type_pile.h` the stack has a fixed capacity `#define TAILLE_MAX 100`.
The current implementation does not check this bound. Therefore a program
containing, for example, 200 `1` tokens could be read and executed without the
package returning a controlled error: behavior becomes undefined (memory
corruption, crash, or wrong results) instead of producing a managed error.

# TP7

## Ex.1 / Ex.2

Complete the `generation_aleatoire` function in the `generation_terrains`
module. Complete `test_generation_terrains.c` to generate multiple terrains and
write terrains and statistics to a result file.

Usage of `test_generation_terrains`:

```bash
Usage: %s <N> <width> <height> <obstacle_density> <result_file>
```

## Ex.3

`curiosity-perf program_file N L H d seed nb_step_max result_file`

prints statistics to the console.

## Ex.4
Example programs that make the robot try to leave any terrain:

**ficher_programme_1.txt:**
Advance indefinitely
```text
{ A C ! } C !
```

**ficher_programme_2.txt:**
Measure the cell ahead and advance if free; turn left if blocked
```text
{ 1 M { G } { A } ? C ! } C !
```

**ficher_programme_3.txt:**
Avoid obstacles by turning right
```text
{ M { A } { D } ? C ! } C !
```

**ficher_programme_4.txt:**
Turn until you find a free cell, then advance.
Logic: measure ahead (1 M). If there is an obstacle → turn left (G) and try
again immediately (measure again). If free → advance.
```text
{ 1 M { G C ! } { A C ! } ? } C !
```

## Ex.5

**ficher_programme_1.txt:**
Description: “advance indefinitely” — it never changes direction.
Terrain where it exits (S):
1×5 row: `C . . . .` → the robot moves straight until it crosses the border →
EXIT.
Terrain where it gets blocked or crashes (O):
1×3 row: `C # .` → robot tries to advance, meets `#` → CRASH.

Characteristic: works only if starting direction has a continuous free path to
the border. It does not detour around obstacles.

**ficher_programme_2.txt:**
Program summary: measure ahead (1 M); if obstacle → G (left), else → A
(advance). Loop.
Terrain where it exits: small maze where left-bias leads to the border.
Terrain where it becomes stuck: examples with a ring of rocks or cycles.

Characteristic: reactive algorithm with left bias. Solves some mazes but can
enter local cycles.

**ficher_programme_3.txt:**
Same as 2 but with right bias (D instead of G).

**ficher_programme_4.txt:**
Program summary: measure ahead (1 M); if obstacle → turn left and retry; if
free → advance. (Turns until it finds a free cell, then advances.)

## Ex.6

Change `#define TAILLE_MAX 100` to `#define TAILLE_MAX 100000` to increase stack
capacity for robustness tests.

Tests comparing `fichier_programme_3` and `fichier_programme_4` [example runs]
are included in the repository and summarized in the README.

# TP8

## Ex.1–3
Added `observateur` package (`observateur.h` / `observateur.c`) and adjusted
`environnement`. Implemented the `curiosity-obs` tool.

Usage:
```bash
curiosity-obs <terrain_file> <robot_program>
```
Prints whether the monitored property holds during execution.

## Ex.4 — test suites
Test suites covering four categories:
- correct programs accepted by the observer;
- incorrect programs rejected by the observer;
- correct programs but rejected (false positives);
- incorrect programs but accepted (observer limitations).

Tests and main script are in `test_curiosity_obs` (`tests_curiosity_obs.sh`).

## Ex.5 — Spin observer
Added `observateur_spin.h/.c` and an `EtatSpin etat_obs_spin` field in
`Environnement`. This observer detects excessive rotations: it rejects any
sequence of 4 consecutive rotations (`G` or `D`).

Example tests:
1) correct program accepted — `prog_spin_accept_ok.prg`
2) program violating spin — `prog_spin_reject_incorrect.prg`
3) program logically acceptable but rejected for 4 rotations —
   `prog_spin_correct_reject.prg`
4) incorrect program (crash) but no spin violation —
   `prog_spin_incorrect_accept.prg`

To run spin tests (from `test_curiosity_obs`):
```bash
./tests_spin.sh
```

In short: `curiosity-obs` runs a (terrain, program) pair and dynamically
verifies properties via observers (including `observateur_spin`).
