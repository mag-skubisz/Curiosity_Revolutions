#include "terrain.h"
#include <stdio.h>

int main(int argc, char **argv) {
  FILE *f;
  Terrain t;
  int x, y;
  erreur_terrain err;

  if (argc < 2) {
    fprintf(stderr, "Usage : %s <fichier>\n", argv[0]);
    return 1;
  }

  f = fopen(argv[1], "r");
  if (!f) {
    fprintf(stderr, "Erreur ouverture fichier '%s'\n", argv[1]);
    return 1;
  }

  err = lire_terrain(f, &t, &x, &y);
  fclose(f);

  if (err != OK) {
    fprintf(stderr, "Erreur lecture fichier '%s'\n", argv[1]);
    return 1;
  }

  afficher_terrain(&t);
  printf("Position initiale du robot : (%d, %d)\n", x, y);
  return 0;
}