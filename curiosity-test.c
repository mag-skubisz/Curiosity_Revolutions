#include "environnement.h"
#include "interprete.h"
#include "programme.h"
#include "robot.h"
#include "terrain.h"
#include "type_pile.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void gestion_erreur_terrain(erreur_terrain e) {
  switch (e) {
  case OK:
    break;
  case ERREUR_FICHIER:
    printf("Erreur lecture du terrain : erreur d'ouverture du fichier\n");
    exit(1);
  case ERREUR_LECTURE_LARGEUR:
    printf("Erreur lecture du terrain : erreur de lecture de la largeur\n");
    exit(1);
  case ERREUR_LECTURE_HAUTEUR:
    printf("Erreur lecture du terrain : erreur de lecture de la hauteur\n");
    exit(1);
  case ERREUR_LARGEUR_INCORRECTE:
    printf("Erreur lecture du terrain : largeur incorrecte\n");
    exit(1);
  case ERREUR_HAUTEUR_INCORRECTE:
    printf("Erreur lecture du terrain : hauteur incorrecte\n");
    exit(1);
  case ERREUR_CARACTERE_INCORRECT:
    printf("Erreur lecture du terrain : caractère incorrect\n");
    exit(1);
  case ERREUR_LIGNE_TROP_LONGUE:
    printf("Erreur lecture du terrain : ligne trop longue\n");
    exit(1);
  case ERREUR_LIGNE_TROP_COURTE:
    printf("Erreur lecture du terrain : ligne trop courte\n");
    exit(1);
  case ERREUR_LIGNES_MANQUANTES:
    printf("Erreur lecture du terrain : lignes manquantes\n");
    exit(1);
  case ERREUR_POSITION_ROBOT_MANQUANTE:
    printf(
        "Erreur lecture du terrain : position initiale du robot manquante\n");
    exit(1);
  }
}

void affichage_position_programme(erreur_programme e) {
  int i;
  printf("Ligne %d, colonne %d :\n", e.num_ligne, e.num_colonne);
  printf("%s\n", e.ligne);
  /* Impression de e.num_colonne-1 espaces */
  for (i = 1; i < e.num_colonne; i++) {
    printf(" ");
  }
  /* Impression d'un curseur de position */
  printf("^\n");
}

void gestion_erreur_programme(erreur_programme e) {
  switch (e.type_err) {
  case OK_PROGRAMME:
    break;
  case ERREUR_FICHIER_PROGRAMME:
    printf("Erreur lecture du programme : erreur d'ouverture du fichier\n");
    exit(2);
  case ERREUR_BLOC_NON_FERME:
    printf("Erreur lecture du programme : bloc non fermé\n");
    exit(2);
  case ERREUR_FERMETURE_BLOC_EXCEDENTAIRE:
    printf("Erreur lecture du programme : fermeture de bloc excédentaire\n");
    affichage_position_programme(e);
    exit(2);
  case ERREUR_COMMANDE_INCORRECTE:
    printf("Erreur lecture du programme : commande incorrecte\n");
    affichage_position_programme(e);
    exit(2);
  }
}

int main(int argc, char **argv) {

  // ON VERIFIE SI ON A UNTEST
  if (argc < 2) {
    printf("Usage: %s <test>\n", argv[0]);
    return 1;
  }

  // OUVERTURE DU FICHIER DE TEST
  FILE *f = fopen(argv[1], "r");
  if (!f) {
    perror("Erreur ouverture fichier de test");
    return 2;
  }

  char terrain[512] = {0};
  char programme[512] = {0};
  int nb_de_pas = 0;
  char e = '\0';
  int x = 0, y = 0;
  Orientation o;

  /* 1) terrain */
  if (fscanf(f, " %511s", terrain) != 1) { fprintf(stderr, "EOF terrain\n"); return 3; }
  printf("Terrain: '%s'\n", terrain);

  /* 2) programme */
  if (fscanf(f, " %511s", programme) != 1) { fprintf(stderr, "EOF programme\n"); return 3; }
  printf("Programme: '%s'\n", programme);

  /* 3) max steps */
  if (fscanf(f, " %d", &nb_de_pas) != 1) { fprintf(stderr, "Nombre de pas n'est pas correcte\n"); return 3; }
  printf("Nombre de pas: %d\n", nb_de_pas);

  /* 4) expected event - read a single non-whitespace char */
  if (fscanf(f, " %c", &e) != 1) { fprintf(stderr, "EOF e\n"); return 3; }
  printf("Evenement attendu: '%c'\n", e);

  /* SI N OU F ON LIT X Y O */
  if (e == 'N' || e == 'F') {
    if (fscanf(f, " %d %d", &x, &y) != 2) { fprintf(stderr, "il nous manque x et y\n"); return 3; }
    /* read orientation as a character and map to enum */
    char o_ch = '\0';
    if (fscanf(f, " %c", &o_ch) != 1) { fprintf(stderr, "il nous manque orientation \n"); return 3; }
    switch (o_ch) {
    case 'N': o = Nord; break;
    case 'E': o = Est; break;
    case 'S': o = Sud; break;
    case 'O': o = Ouest; break;
    default:
      fprintf(stderr, "Orientation invalide: '%c'\n", o_ch);
      return 3;
    }
    printf("Position finale attendue: x=%d y=%d o=%c\n", x, y, o_ch);
  }
  fclose(f);


  /* Initialisation de l'environnement */
  Environnement envt;
  Programme prog;
  erreur_terrain errt;
  erreur_programme errp;
  etat_inter etat;
  resultat_inter res;

  errt = initialise_environnement(&envt, terrain);
  gestion_erreur_terrain(errt);

  errp = lire_programme(&prog, programme);
  gestion_erreur_programme(errp);

  int x1 = envt.r.x;
  int y1 = envt.r.y;
  Orientation o1 = envt.r.o;
  int nb_de_pas1=0;

  init_etat(&etat);
  do {
    res = exec_pas(&prog, &envt, &etat);
    position(&envt.r, &x1, &y1);
    o1 = orient(&envt.r);
    nb_de_pas1++;
  } while (res == OK_ROBOT);


  /* JE PRINT L'environnement POUR VERIFICATION*/
  printf("Environnement: \n");
  afficher_envt(&envt);

  //VERIFICATION DES RESULTATS
  int ok = 1;
  // nombre de pas
  if (nb_de_pas1 != nb_de_pas) {
    printf("TEST %s : FAILED (nombre de pas maximum dépassé)\n", argv[1]);
    ok = 0;
  }
  // evenement final
  if ( (e == 'N' && res != OK_ROBOT) ||
       (e == 'O' && res != CRASH_ROBOT) ||
       (e == 'F' && res != ARRET_ROBOT) ||
       (e == 'S' && res != SORTIE_ROBOT) ||
       (e == 'P' && res != PLOUF_ROBOT) ) {
        printf("TEST %s : FAILED (mauvais evenement final)\n", argv[1]);
        ok =0; 
    }
  // x,y
  if (e == 'N' || e == 'F') {
      if (x1 == x && y1 == y) {
        printf("TEST %s : FAILED (mauvaise position finale)\n", argv[1]);
        ok = 0;
      }
  // orientation
  if (o1 != o) {
    printf("Orientation finale attendue: %u, obtenue: %u\n", o, o1);
    printf("TEST %s : FAILED (mauvaise orientation finale)\n", argv[1]);
    ok = 0;
  }
  }
  // RESULTAT FINAL
  if (ok) {
    printf("TEST %s : OK\n", argv[1]);
    return 0;
  } else {
    return 1; }

}
