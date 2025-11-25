/* Observateur pour détecter une rotation excessive :
   Pas plus de 3 rotations consécutives (G ou D) sans A ou M entre.
*/
#ifndef _OBSERVATEUR_SPIN_H_
#define _OBSERVATEUR_SPIN_H_

typedef enum {S0, S1, S2, S3, SPIN_ERR} EtatSpin;
typedef enum {ACT_A, ACT_M, ACT_D, ACT_G} ActionSpin;

EtatSpin initiale_obs_spin();
EtatSpin transition_obs_spin(EtatSpin e, ActionSpin a);
int est_accepte_obs_spin(EtatSpin e);

#endif
