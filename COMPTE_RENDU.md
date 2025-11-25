# Compte‑rendu pour la soutenance 

## TP6 — démarche pour écrire les tests
--------------------------------------

**Objectif :** 
- couvrir les comportements finaux possibles du robot (sortie, crash, plouf, blocage, arrêt ..) et tester les opérations du langage (arithmétique, clone, rotation, boucle, condbloc, mesure, etc.).
Il faut tester tous les cas dans le switch dans le dossier interprete. Pour cela, on a cree 15 tests certains testant deux choses à la fois (par exemple, add teste aussi empile nb etc.)


**Méthode pour creation des tests :**
- Pour tester correctement chaque cas dans le switch de l’interpréteur, nous avons essayé de créer des tests très univoques. Par exemple, si l’on veut vérifier que le robot se déplace correctement en testant s’il tombe dans l’eau après avoir effectué un pas en avant (sur un terrain où il y a de l’eau devant le robot), on ne peut pas placer de l’eau tout autour du robot, car il pourrait tomber dans l’eau même si son déplacement est incorrect, simplement à cause de l’environnement.
De même, pour tester des opérations comme sub ou div, ces opérations ne sont pas commutatives. Mais si l’on fournit deux nombres identiques sur la pile, on obtiendra le même résultat. Dans ce cas, on ne teste pas réellement si, par exemple, la fonction soustrait les nombres dans le mauvais ordre.
- Nous avons également essayé de créer des tests courts et simples, testant plutôt une seule chose à la fois, car parfois, si plusieurs erreurs apparaissent dans l’interprétation du programme, ces erreurs peuvent se masquer les unes les autres. C’est pourquoi nos terrains sont généralement petits et nos programmes courts.

**Méthode technique :**
- Définition d'un format `test_*.txt` décrivant : fichier terrain, programme, pas maximum, événement attendu (N/F/S/O/P) et, si utile, position/orientation attendues.
- Pour chaque test : exécution via `curiosity-test.sh`
- Pour tout tester pour chaque interprete, il faut utiliser le script : `run_all_curiosity_tests.sh`

**Résultats :**
Au final, nous avons constaté que seuls les interprètes 1 et 7 sont corrects.

## TP7 — programmes‑robots étudiés et évaluation statistique
----------------------------------------------------------

**Programmes choisis:**

- Programme A (fichier : `ficher_programme_3.txt`) — stratégie "biais droit": mesure l'avant et tourne à droite en cas d'obstacle ; comportement simple, réactif, favorise les détours à droite.
- Programme B (fichier : `ficher_programme_4.txt`) — stratégie "tourne jusqu'à trouver libre" : mesure l'avant ; si obstacle → tourne à gauche et répète ; sinon → avance. Plus exploratoire localement.

**Évaluation statistique : méthode**

- Génération d'ensembles de terrains aléatoires (paramètres : largeur L, hauteur H, densité d'obstacles d, nombre de terrains N). Pour chaque couple (terrain, programme) on exécute `curiosity-perf` jusqu'à `nb_step_max` et on collecte l'événement final (S/O/P/N/F) et le nombre de pas quand applicable.
- Calcul des pourcentages par événement et des moyennes de pas pour les sorties.

**Résultats (exemples tirés des runs effectués) :**

1) N=20, L=25, H=25, d=0.4, nb_step_max=10000

- `fichier_programme_3.txt` : Sorties 0 (0.00%), Bloqués 13 (65.00%), Plouf 1 (5.00%), Crash 6 (30.00%).
- `fichier_programme_4.txt` : Sorties 1 (5.00%), Bloqués 19 (95.00%), Plouf 0, Crash 0.

2) N=20, L=9, H=9, d=0.7, nb_step_max=10000

- `fichier_programme_3.txt` : Sorties 0 (0.00%), Bloqués 10 (50.00%), Plouf 2 (10.00%), Crash 8 (40.00%).
- `fichier_programme_4.txt` : Sorties 5 (25.00%), Bloqués 15 (75.00%), Plouf 0, Crash 0.

**Interprétation synthétique :**
- Le programme B (tourne jusqu'à trouver libre) obtient plus de sorties sur petits terrains denses (L=9,H=9,d=0.7) que le programme A ; il se comporte mieux en exploration locale mais peut rester bloqué globalement.
- Le programme A (biais droit) est plus sensible aux configurations bloquantes et au placement d'obstacles, menant à davantage de crashs ou blocages dans terrains larges et denses.
- Cependant, les deux programmes peuvent rencontrer des problèmes pour sortir de terrains très grands et complexes, car ils n’ont pas de mémoire. Ainsi, même si le programme B se comporte nettement mieux, il peut se boucler et répéter les mêmes configurations de mouvements sur le terrain.

## TP8 — observateur défini et automate d'états finis
---------------------------------------------------

**Observateur implémenté :** observateur "spin" — détecte les rotations excessives répétées (comportement en boucle sur G/D).

*Spécification :*
- Objectif : rejeter toute séquence de 4 rotations consécutives (G ou D).
- États : compteur c ∈ {0,1,2,3,>=4} (ou états S0..S4)
- Initialisation : c = 0 (acceptant).
- Transitions :
  - action ∈ {G,D} → c := min(c+1,4)
  - action ∉ {G,D} → c := 0
- Acceptation : accepté si c < 4 ; rejet si c = 4 (violation détectée).

*Automate :*

- S0 --G/D--> S1 --G/D--> S2 --G/D--> S3 --G/D--> S4(rejet)
- Sx --autre--> S0

*Intégration :* l'environnement appelle la fonction de transition de l'observateur à chaque action observable (A, M, G, D, ...). `curiosity-obs` interroge ensuite l'état d'acceptation après chaque pas pour signaler une violation.

*Conclusion:*
----------------------------

Les tests écrits permettent de vérifier les éléments fondamentaux de l’interpréteur et de s’assurer que le langage est bien défini.

La stratégie de navigation influence fortement les performances du robot (combien de fois il réussit à sortir du terrain). La stratégie serait la plus efficace si le robot avait, au moins en partie, la capacité de mémoriser les endroits déjà visités.

Les observateurs, observateur et observateur spin, sont capables d’évaluer les chances des programmes de guider correctement le robot pour qu’il sorte du terrain.

L’observateur vérifie que nous avons effectué une mesure avant le mouvement, ce qui protège contre la chute dans l’eau ou les collisions avec les rochers.

L’observateur spin vérifie que le robot ne se déplace pas en boucle et s’assure, le cas échéant, de l’arrêt du programme pour éviter les cycles infinis.