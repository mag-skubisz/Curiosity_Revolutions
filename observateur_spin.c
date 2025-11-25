#include "observateur_spin.h"

/* Automate : compte le nombre de rotations consécutives (G ou D).
   S0 : 0 rotations récentes
   S1 : 1 rotation
   S2 : 2 rotations
   S3 : 3 rotations
   SPIN_ERR : >=4 rotations consécutives -> violation
*/

EtatSpin initiale_obs_spin() { return S0; }

EtatSpin transition_obs_spin(EtatSpin e, ActionSpin a) {
  switch (e) {
  case S0:
    if (a == ACT_D || a == ACT_G) return S1;
    return S0; // A ou M
  case S1:
    if (a == ACT_D || a == ACT_G) return S2;
    return S0;
  case S2:
    if (a == ACT_D || a == ACT_G) return S3;
    return S0;
  case S3:
    if (a == ACT_D || a == ACT_G) return SPIN_ERR;
    return S0;
  case SPIN_ERR:
  default:
    return SPIN_ERR;
  }
}

int est_accepte_obs_spin(EtatSpin e) { return e != SPIN_ERR; }
