#include <stdio.h>
#include <stdlib.h>
#include "environnement.h"
#include "programme.h"
#include "interprete.h"
#include "observateur.h"
#include "observateur_spin.h"

// pour n'avoir pas un boucle infini ou trop long
#define STEP_LIMIT 1000000


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
  //VERIFICATION DES ARGUMENTS
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <fichier_terrain> <fichier_programme>\n", argv[0]);
    return 1;
  }
  
  char *terrain = argv[1];
  char *programme = argv[2];
  printf("Terrain: '%s'\n", terrain);
  printf("Programme: '%s'\n", programme);

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

  int steps = 0;
  int violation_detected_am = 0;
  int violation_detected_spin = 0;

  init_etat(&etat);
  do {
    res = exec_pas(&prog, &envt, &etat);
    /* vérifier l'état de l'observateur stocké dans l'environnement */
    if (!est_accepte_obs(envt.etat_obs) && !violation_detected_am) {
      printf("OBSERVATEUR: INCORRECT -> violation détectée (A sans M)\n");
      violation_detected_am++;
      //break;
    }
    if (!est_accepte_obs_spin(envt.etat_obs_spin) && !violation_detected_spin) {
      printf("OBSERVATEUR_SPIN: INCORRECT -> violation détectée (trop de rotations consécutives)\n");
      violation_detected_spin++;
      //break;
    }
    steps++;
  } while (res == OK_ROBOT && steps < STEP_LIMIT);

  //afficher le raison d'arreter le programme
  if (!violation_detected_am) {
    printf("OBSERVATEUR: CORRECT -> n'est pas détectée (A sans M)\n");
  }
  if (!violation_detected_spin) {
    printf("OBSERVATEUR_SPIN: CORRECT -> n'est pas détectée (trop de rotations consécutives)\n");
  }
  return 0;
}