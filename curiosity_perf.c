#include "environnement.h"
#include "interprete.h"
#include "programme.h"
#include "robot.h"
#include "terrain.h"
#include "type_pile.h"
#include "generation_terrains.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define NB_STEPS_MAX 1000000
#define GRAINE_MAX 100000000
/* codes de resultats */
#define RES_SORTIE 0
#define RES_BLOQUE -1
#define RES_PLOUF -2
#define RES_CRASH -3

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
// on verifie la nombre d'arguments
  if (argc < 9) {
    printf("Usage: %s <fichier_programme> N L H d graine nb_step_max fichier_res\n", argv[0]);
    return 1;
  }

// donnes necessaires
  int N, l, h, nb_step_max;
  float d; 
  unsigned int graine;
  FILE *resFile;

  N = strtol(argv[2], NULL, 10);
  l = strtol(argv[3], NULL, 10);
  h = strtol(argv[4], NULL, 10);
  d = strtof(argv[5], NULL);
  graine = strtof(argv[6], NULL);
  nb_step_max = strtol(argv[7], NULL, 10);

// on verifier si les donnes sont correcrs
  FILE *f = fopen(argv[1], "r");
  if (!f) {
    perror("Erreur ouverture fichier de programme");
    return 2;
  }
  if (l > DIM_MAX || l % 2 == 0) {
    printf("Largeur incorrecte : doit être impaire et <= %d\n", DIM_MAX);
    return 1;
  }
  if (h > DIM_MAX || h % 2 == 0) {
    printf("Hauteur incorrecte : doit être impaire et <= %d\n", DIM_MAX);
    return 1;
  }
  if ((d > 1) || (d < 0)) {
    printf("Densité incorrecte : doit être comprise entre 0 et 1\n");
    return 1;
  }
  if (nb_step_max <= 0 || nb_step_max > NB_STEPS_MAX) {
    printf("Nombre de pas maximum incorrect : doit être compris entre 1 et %d\n", NB_STEPS_MAX);
    return 1;
  }
  if (N <= 0) {
    printf("Nombre de terrains incorrect : doit être un entier positif\n");
    return 1;
  }
  if (graine <=0 || graine > GRAINE_MAX) {
    printf("Graine incorrecte : doit être un entier compris entre 1 et %d\n", GRAINE_MAX);
    return 1;
  }

// Initialisation de la fonction random
  srand(graine);
// Ouverture du fichier résultat
  resFile = fopen(argv[8], "w");
  if (!resFile) { perror("Erreur ouverture fichier_res"); return 1; }
// Écriture du nombre de terrains
  fprintf(resFile, "%d\n", N);

// preparation du programme
  Programme prog;
  erreur_programme errp = lire_programme(&prog, argv[1]);
  if (errp.type_err != OK_PROGRAMME) {
    gestion_erreur_programme(errp); /* affiche l'erreur */
  }

// statistics
  int count_sortie = 0, 
  count_bloque = 0, 
  count_plouf = 0, 
  count_crash = 0;
  long sum_steps_sortie = 0;

// BOUCLE PRINCIPALE
  for (int i = 0; i < N; i++) {
    Terrain T;
    generation_aleatoire(&T, l, h, d);
    // si le terrain n'est pas correct, on le regénère
    if (!existe_chemin_vers_sortie(&T)) {
      //printf("Erreur: pas de chemin entre le centre et le bord\n");
      i --;
    } else {
      // on continue
      // ON CREE UN FICHER TEMPORAIRE POUR ECRIRE LE TERRAIN
      char tmpname[] = "/tmp/curio_terrain_XXXXXX"; // SUFIX -> nom du ficher
      int tmpfd = mkstemp(tmpname);
      if (tmpfd == -1) {
        perror("mkstemp");
        fclose(resFile);
        return 1;
      }
      close(tmpfd);

      FILE *tf = fopen(tmpname, "w");
      if (!tf) {
        perror("fopen tmp");
        unlink(tmpname);
        fclose(resFile);
        return 1;
      }

      /* on ecrit le terrain dans le fichier temporaire */
      fprintf(tf, "%d\n%d\n", l, h);
      int rx = l / 2, ry = h / 2;
      for (int yy = 0; yy < h; yy++) {
        for (int xx = 0; xx < l; xx++) {
          if (xx == rx && yy == ry) {
            fputc('C', tf);
          } else {
            switch (T.tab[xx][yy]) {
            case LIBRE:  fputc('.', tf); break;
            case ROCHER: fputc('#', tf); break;
            case EAU:    fputc('~', tf); break;
            default:     fputc('?', tf); break;
            }
          }
        }
        fputc('\n', tf);
      }
      fclose(tf);

      /* Initialisation de l'environnement */
      Environnement envt;
      erreur_terrain errt = initialise_environnement(&envt, tmpname);
      if (errt != OK) {
        unlink(tmpname); //?
        gestion_erreur_terrain(errt);
      }
      unlink(tmpname);
      // initialisation de l'état de l'interpréteur
      // on execute les pas de que on a le status OK_ROBOT ou on a depasse nb_step_max
      etat_inter etat;
      init_etat(&etat);

      resultat_inter res = OK_ROBOT;
      int nb_de_pas1 = 0;
      do {
        res = exec_pas(&prog, &envt, &etat);
        nb_de_pas1++;
      } while ((res == OK_ROBOT) && nb_de_pas1 < nb_step_max);

      /* interpretation des resultats et ajout des statistiques */
      if (res == SORTIE_ROBOT) {
        fprintf(resFile, "%d\n", nb_de_pas1);
        count_sortie++;
        sum_steps_sortie += nb_de_pas1;
      } else if (res == PLOUF_ROBOT) {
        fprintf(resFile, "%d\n", RES_PLOUF); 
        count_plouf++;
      } else if (res == CRASH_ROBOT) {
        fprintf(resFile, "%d\n", RES_CRASH); 
        count_crash++;
      } else { /* ARRET_ROBOT ou limite de pas atteinte */
        fprintf(resFile, "%d\n", RES_BLOQUE); 
        count_bloque++;
      }
    }
  }
  fclose(resFile);  

  
  // Écriture/Affichage des statistiques
  printf("Terrains: %d\n", N);
  printf("Sorties : %d (%.2f%%)\n", count_sortie, 100.0 * (double)count_sortie / N);
  printf("Bloqués : %d (%.2f%%)\n", count_bloque, 100.0 * (double)count_bloque / N);
  printf("Plouf (eau) : %d (%.2f%%)\n", count_plouf, 100.0 * (double)count_plouf / N);
  printf("Crash (rocher) : %d (%.2f%%)\n", count_crash, 100.0 * (double)count_crash / N);
  if (count_sortie > 0) {
    printf("Moyenne pas pour sorties : %.2f\n", (double)sum_steps_sortie / count_sortie);
  } else {
    printf("Aucune sortie, moyenne pas non disponible\n");
  }

  return 0;

}