# Project report

## TP6 — approach for writing tests
-----------------------------------

**Goal:**
- Cover the possible terminal behaviors of the robot (exit, crash, fall into
  water, stuck, normal stop) and test the language operations (arithmetic,
  clone, rotation, loop, conditional block, measure, etc.). We aimed to test
  every case in the interpreter's switch. We created ~15 tests; some tests
  target two features at once (e.g., Add also tests pushing numbers).

**Test design method:**
- To test each switch case in the interpreter precisely, we created unambiguous
  tests. For instance, to verify that moving forward causes a fall into water
  when there is water in front, we avoid placing water around the robot so
  that the environment itself does not mask an implementation error.
- For non‑commutative operations like Sub or Div, we avoid using identical
  operands in tests (which would hide order errors).
- We prefer short, focused tests (small terrains, short programs) that check
  one behavior at a time; this reduces error masking between multiple faults.

**Technical method:**
- A `test_*.txt` format describes: terrain file, program file, max steps,
  expected event (N/F/S/O/P) and optional expected coordinates/orientation.
- Each test is run with `curiosity-test.sh`.
- To run tests for every interpreter variant use `run_all_curiosity_tests.sh`.

**Results:**
- Only interpreters 1 and 7 are correct; the others contain small one-line
  issues discovered by the tests.

## TP7 — robot programs studied and statistical evaluation
---------------------------------------------------------

**Chosen programs:**
- Program A (`ficher_programme_3.txt`): right‑bias strategy — measure ahead
  and turn right if blocked. Simple, reactive, favors right turns.
- Program B (`ficher_programme_4.txt`): "turn until free" strategy — measure
  ahead; if blocked → turn left and repeat; else → advance. More explorative
  locally.

**Statistical evaluation method:**
- Generate sets of random terrains (parameters: width L, height H, obstacle
  density d, number of terrains N). For each (terrain, program) pair run
  `curiosity-perf` up to `nb_step_max` and record the terminal event (S/O/P/N/F)
  and step counts when applicable.
- Compute percentages per event and average steps for exits.

**Example results (from performed runs):**

1) N=20, L=25, H=25, d=0.4, nb_step_max=10000
- `fichier_programme_3.txt`: Exits 0 (0.00%), Stuck 13 (65.00%), Fall 1 (5.00%), Crash 6 (30.00%).
- `fichier_programme_4.txt`: Exits 1 (5.00%), Stuck 19 (95.00%), Fall 0, Crash 0.

2) N=20, L=9, H=9, d=0.7, nb_step_max=10000
- `fichier_programme_3.txt`: Exits 0 (0.00%), Stuck 10 (50.00%), Fall 2 (10.00%), Crash 8 (40.00%).
- `fichier_programme_4.txt`: Exits 5 (25.00%), Stuck 15 (75.00%), Fall 0, Crash 0.

**Short interpretation:**
- Program B (turn‑until‑free) produces more exits on small dense terrains
  (L=9,H=9,d=0.7) than Program A: it explores locally better but can still be
  globally trapped.
- Program A (right bias) is more sensitive to blocking configurations and
  tends to produce more crashes or stuck cases in large/dense terrains.

Note: the numbers above are sample run outputs. For the final submission,
attach the `curiosity-perf` summaries (CSV/TXT) and the RNG seeds used.

## TP8 — defined observer and finite‑state automaton
--------------------------------------------------

**Implemented observer:** the "spin" observer — it detects repeated
excessive rotations (looping behavior on G/D).

Specification:
- Objective: reject any sequence of 4 consecutive rotations (G or D).
- State: counter c ∈ {0,1,2,3,>=4} (or states S0..S4).
- Initialization: c = 0 (accepting).
- Transitions:
  - action ∈ {G,D} → c := min(c+1,4)
  - action ∉ {G,D} → c := 0
- Acceptance: accepted if c < 4; reject if c = 4 (violation detected).

Automaton (compact):
- S0 --G/D--> S1 --G/D--> S2 --G/D--> S3 --G/D--> S4(reject)
- Sx --other--> S0

Integration: the environment calls the observer's transition function at each
observable action (A, M, G, D, ...). `curiosity-obs` checks acceptance after
each step to report violations.

## Conclusion
-------------

The project implements an end‑to‑end validation chain: program parsing,
simulation on terrains, unit and functional tests, and dynamic observers. The
tests validate interpreter primitives; experimental statistics show that
navigation strategy heavily affects success depending on terrain size and
density. The "spin" observer demonstrates how a simple temporal property can
be monitored at runtime to prevent infinite looping.

## Files to submit on Moodle
- Source archive (zip/tar.gz).
- All `test_*.txt` files and scripts (`tests_curiosity.sh`, `tests_curiosity_obs.sh`, `tests_spin.sh`, etc.).
- `curiosity-perf` run summaries (CSV or TXT) and RNG seeds/parameters.

---
This English version mirrors the French `COMPTE_RENDU.md` content and is
ready to export to PDF (≤ 2 pages) for submission. Fill in student names and
attach the run artifacts before uploading.
