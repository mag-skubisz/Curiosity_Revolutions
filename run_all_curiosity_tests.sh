#!/usr/bin/env bash
# Construire et exécuter curiosity-test0..curiosity-test9 sur tous les fichiers test_*.txt
# Pour chaque i : lancer `make curiosity-test$i`, puis `./curiosity-test$i test_*.txt`

set -euo pipefail # je lus que c'est une bonne pratique de securité de script bash

ROOT_DIR="$(pwd)"
echo "Exécution des tests depuis : $ROOT_DIR"

any_summary=0

for i in {0..9}; do
  target="curiosity-test${i}"
  echo
  echo "==============================================="
  echo "CIBLE : $target"
  echo "-----------------------------------------------"

  if make "$target"; then
  echo "OK : $target"
  else
  echo "ÉCHEC : $target -- tests ignorés pour cette cible"
  summary="$target : ÉCHEC"
  echo "$summary"
    any_summary=1
    continue
  fi

  # Ensure executable exists
  if [ ! -x "./$target" ]; then
  echo "Exécutable ./$target introuvable ou non exécutable -- tests ignorés"
    any_summary=1
    continue
  fi

  passed=0
  total=0

  # iterate tests
  shopt -s nullglob # tableau vide si aucun fichier ne correspond
  tests=( test_*.txt )
  shopt -u nullglob # désactiver nullglob

  if [ ${#tests[@]} -eq 0 ]; then
    echo "Aucun fichier test_*.txt trouvé dans $ROOT_DIR"
    any_summary=1
    continue
  fi

  for t in "${tests[@]}"; do
    printf "[%s] Exécution du test : %s\n" "$target" "$t"
    if ./$target "$t" >/dev/null 2>&1; then
      printf "[%s] %s : RÉUSSI\n" "$target" "$t"
      passed=$((passed+1))
    else
      printf "[%s] %s : ÉCHOUÉ\n" "$target" "$t"
    fi
    total=$((total+1))
  done

  if [ "$total" -eq 0 ]; then
  echo "Aucun test exécuté pour $target"
    any_summary=1
  else
    if [ "$passed" -eq "$total" ]; then
      echo "RÉSULTAT : $target : TOUS LES TESTS RÉUSSIS ($passed/$total)"
    else
      echo "RÉSULTAT : $target : $passed/$total tests réussis"
      any_summary=1
    fi
  fi
done

if [ "$any_summary" -eq 0 ]; then
  echo
  echo "Toutes les cibles compilées et tous les tests réussis."
  exit 0
else
  echo
  echo "Certaines compilations/tests ont échoué. Vérifiez la sortie ci‑dessus."
  exit 1
fi
