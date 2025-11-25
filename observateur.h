/* Observateur :
   "avant d'avancer, une mesure est toujours effectuée".
*/
#ifndef _OBSERVATEUR_H_
#define _OBSERVATEUR_H_

/* Evénements observables et actions*/
typedef enum {Ini, Mes, Err} Etat;
typedef enum {A, M, D, G} Action;

/* Initialiser l'observatuer */
Etat initiale_obs();
/*Transitions */
Etat transition_obs(Etat e, Action a);
/* Verification d'acceptation */
int est_accepte_obs(Etat e);

#endif
