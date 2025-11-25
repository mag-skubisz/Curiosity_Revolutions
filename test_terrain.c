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
    switch (err) {
    case ERREUR_FICHIER:
      fprintf(stderr, "Erreur : fichier invalide ou impossible à lire '%s'\n",
              argv[1]);
      break;
    case ERREUR_LECTURE_LARGEUR:
      fprintf(stderr, "Erreur : lecture de la largeur du terrain échouée\n");
      break;
    case ERREUR_LECTURE_HAUTEUR:
      fprintf(stderr, "Erreur : lecture de la hauteur du terrain échouée\n");
      break;
    case ERREUR_LARGEUR_INCORRECTE:
      fprintf(stderr, "Erreur : largeur incorrecte (hors limites)\n");
      break;
    case ERREUR_HAUTEUR_INCORRECTE:
      fprintf(stderr, "Erreur : hauteur incorrecte (hors limites)\n");
      break;
    case ERREUR_CARACTERE_INCORRECT:
      fprintf(stderr,
              "Erreur : caractère incorrect trouvé dans le fichier du terrain\n");
      break;
    case ERREUR_LIGNE_TROP_LONGUE:
      fprintf(stderr, "Erreur : une ligne du terrain est trop longue\n");
      break;
    case ERREUR_LIGNE_TROP_COURTE:
      fprintf(stderr, "Erreur : une ligne du terrain est trop courte\n");
      break;
    case ERREUR_LIGNES_MANQUANTES:
      fprintf(stderr, "Erreur : lignes manquantes dans le fichier du terrain\n");
      break;
    case ERREUR_POSITION_ROBOT_MANQUANTE:
      fprintf(stderr,
              "Erreur : position initiale du robot manquante dans le terrain\n");
      break;
    default:
      fprintf(stderr, "Erreur inconnue lors de la lecture du terrain\n");
    }
    return 1;
  }

  afficher_terrain(&t);
  printf("Position initiale du robot : (%d, %d)\n", x, y);
  return 0;
}