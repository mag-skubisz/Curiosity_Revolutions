/* Implémentation simple de l'observateur décrit dans le sujet.
   Automate:
     - Ini : pas de mesure immédiate
     - Mes : dernier événement = M (mesure OK)
     - Err : violation détectée (A sans M)
*/

#include "observateur.h"
#include <stdio.h>
#include <stdlib.h>

#include "observateur.h"
Etat initiale_obs() { return Ini; }

Etat transition_obs(Etat e, Action a){
	switch (e) {
		case Ini:
			switch (a) {
				case M: return Mes;
				case D: return Ini;
				case G: return Ini;
				case A: return Err;
				default: return Err; }
		case Mes:
			switch (a) {
				case M: return Mes;
				case D: return Ini;
				case G: return Ini;
				case A: return Ini;
				default: return Err; }
		case Err:
			default: return Err; }
}

int est_accepte_obs(Etat e){
	return (e == Ini) || (e == Mes);
}