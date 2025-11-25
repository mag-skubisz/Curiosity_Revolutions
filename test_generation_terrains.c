#include "generation_terrains.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_TIRES_PER_PROGRAMME 100000 // j'ai a remarqué que si, par exemple, l’utilisateur
// définit density d’obstacles à 1, une boucle infinie se crée,
// donc MAX_TIRES_PER_PROGRAMME limite le nombre de tentatives de génération du terrain
// et arrête le programme si une erreur survient

int nb_cases_occupees(Terrain *T) {
  int occupee = 0;
  int x, y;
  for (x = 0; x < largeur(T); x++) {
    for (y = 0; y < hauteur(T); y++) {
      if (T->tab[x][y] != LIBRE)
        occupee++;
    }
  }
  return occupee;
}

// Test de generation aléatoire de terrains
// Le programme génère n terrains de largeur et hauteur fixes
// avec largeur et hauteur impaires et inférieures a dimension_max de terrain.h
// avec densité d'obstacle dObst
// autre que la case centrale soit occupee
// l'appel du programme se fait avec 5 arguments :
// generation_terrains N largeur hauteur dObstacle fichier_res
// la sortie se fait dans le fichier resultat

int main(int argc, char **argv) {
  int N, i, l, h;
  float dObst;
  FILE *resFile;
  Terrain T;

  if (argc < 6) {
    printf(
        "Usage: %s <N> <largeur> <hauteur> <densite_obstacle> <fichier_res> \n",
        argv[0]);
    return 1;
  }

  N = strtol(argv[1], NULL, 10);
  l = strtol(argv[2], NULL, 10);
  h = strtol(argv[3], NULL, 10);
  dObst = strtof(argv[4], NULL);

  // test de l et h
  if (l > DIM_MAX || l % 2 == 0) {
    printf("Largeur incorrecte : doit être impaire et <= %d\n", DIM_MAX);
    return 1;
  }
  if (h > DIM_MAX || h % 2 == 0) {
    printf("Hauteur incorrecte : doit être impaire et <= %d\n", DIM_MAX);
    return 1;
  }
  if ((dObst > 1) || (dObst < 0)) {
    printf("Densité incorrecte : doit être comprise entre 0 et 1\n");
    return 1;
  }

  // Ouverture du fichier résultat
  resFile = fopen(argv[5], "w");
  // Écriture du nombre de terrains
  fprintf(resFile, "%d\n", N);

  // Initialisation de la fonction random
  srand((unsigned int)time(NULL));

  // Génération aléatoire des terrains
  int terrains_incorrec = 0; // Compteur de terrains incorrects
  float densite_totale = 0.0; // Pour calculer la densité moyenne

  int max_tirages = 0;
  int error_id = 1;
  for (i = 0; i < N; i++) {
    int error = 0; // 1 si la case centrale n'est pas libre; 2 si il n'existe pas le chemin entre centre et bord
    generation_aleatoire(&T, l, h, dObst);
    // Vérification de la case centrale
    if (T.tab[l / 2][h / 2] != LIBRE) {
      printf("Erreur %d: la case centrale est occupée\n", error_id);
      error = 1;
      error_id++;
    }
    // Vérification de l'existence d'un chemin entre le centre et le bord
    if (!existe_chemin_vers_sortie(&T)) {
      printf("Erreur %d: pas de chemin entre le centre et le bord\n", error_id);
      error = 2;
      error_id++;
    }

    // Calcul de la densité d'obstacles pour terrains corrects et incorrects
    int occupee = nb_cases_occupees(&T);
    float dens = (float)occupee / (float)(l * h);
    densite_totale += dens;

    // Écriture des terrains générés dans le fichier resFile
    if (error == 0) {
      // fonction added in generation_terrains
      ecrire_terrain_dans_ficher(resFile, &T, l / 2, h / 2);
      // Écriture de la densite d'obstacles
      fprintf(resFile, "%f\n", dens); //densite
      fprintf(resFile, "\n");
    } else {
      terrains_incorrec += 1;
      i --; // Regénérer un terrain correct
    }
    max_tirages ++;
    /* Si le nombre d'essais dépasse la limite, écrire une erreur et arrêter le programme */
    if (max_tirages > MAX_TIRES_PER_PROGRAMME) {
      fprintf(stderr, "ERREUR : nombre maximal d'essais de génération de terrains (%d) dépassé. Arrêt.\n", MAX_TIRES_PER_PROGRAMME);
      fprintf(resFile, "# ERREUR : nombre maximal d'essais de génération de terrains (%d) dépassé. Génération interrompue.\n", MAX_TIRES_PER_PROGRAMME);
      fclose(resFile);
      return 2;
    }
  }  
  // Écriture/Affichage des statistiques
  fprintf(resFile,"Densite moyenne de tous les terrains (corrects et incorrects) %f vs la densite donne %f\n", densite_totale / (N + terrains_incorrec), dObst);
  fprintf(resFile,"Le pourcentage de terrain valides par rapport au nombre total généré: %f\n",( (float)N/ (N + terrains_incorrec) ) * 100);

  // fermeture des fichiers
  fclose(resFile);
  return 0;
}
