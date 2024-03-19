#include "astar.h"
#include <math.h>

//heuristiques

double h_euclidienne (Sommet a, Point final, Couleur c) {
    double max = sqrt(2.)*BOARD_SIZE;
    double dist_eucl = sqrt(pow(abs(final.q - a.positions[c].q),2) + 
                            pow(abs(final.r - a.positions[c].r),2));
    return dist_eucl/max;
}

double h_manhattan (Sommet a, Point final, Couleur c) {
    double max = BOARD_SIZE*2;
    double dist_man = abs(final.q - a.positions[c].q) + abs(final.r - a.positions[c].r);
    return dist_man / max;
} 

double h_rapide (Sommet a, Point final, Couleur c) {
    double dist_man = 0;
    for(int k=0; k<N_Robots; k++) {
        dist_man += abs(final.q - a.positions[k].q) + abs(final.r - a.positions[k].r);
    }
    return dist_man;
}



//fonction principale
Zipper a_star (Sommet s, Point t, Couleur couleur, double (*h) (Sommet ,Point , Couleur) /*fonction heuristique*/,
				 bool VerticalWalls[BOARD_SIZE+1][BOARD_SIZE+1], 
				 bool DiagupWalls[BOARD_SIZE + 1][BOARD_SIZE+1],
				 bool DiagDownWalls[BOARD_SIZE + 1][BOARD_SIZE + 1]) {
   // création de la file avec s de priorité h(s)
    s.dist = 0;
    PMinStack file;
    HashTbl hash = {NULL, mersenne, 0};
    HashDist d = {NULL, mersenne, 0};
    InitHashTbl(&hash);
    InitHashDist(&d);
    InitPMinStack(&file);
    double p = (*h) (s,t, couleur);
    InsererPMinStack(&file,s, p);

    // création de variables utiles
    int tours = 0;
    Sommet v, w;
    Sommet final;
    final.dist = -1;
    SList* descendants = NULL;

     

    while(file.remplissage != 0){
        ExtrairePMinStack(&file,&v);

        if (tours % 100000 == 0) printf("dist : %d ; tours : %d\n", v.dist, tours);
       
        if (v.positions[couleur].q == t.q && v.positions[couleur].r == t.r){
            printf("Nombre total de tours : %d\n", tours);
            FreePMinStack(&file);
            CopieSommet(v, &final);
        } else {
       	    descendants = DescendantsDirects(v, VerticalWalls, DiagupWalls, DiagDownWalls);
            while (descendants != NULL) {
   			    descendants = ExtraireSList(descendants, &w);
                int newdist = RecupererDist(&d, w);
                if (newdist > v.dist + 1){
				    w.dist = v.dist + 1;
                    p = (*h) (w, t, couleur);
                    InsererPMinStack(&file, w, p + w.dist);
                    AjouteHashTbl(w, v, &hash);
                    AjouteHashDist(w, &d);
                }
            }
        }
        tours++;
    }
    FreeHashDist(&d);

    Chemin* chemin = NULL;
    Sommet courant;
    CopieSommet(final, &courant);
	while (!EgaliteSommet(courant, s)) {
		if (courant.dist == -1) {
			//Echec de récupération du sommet
			printf("Echec de récupération d'un sommet\n\n");
			return (Zipper){NULL, NULL, 0};
		} else {
			chemin = AjouterChemin(chemin, courant);
		}
		courant = ChercheHashTbl(&hash, courant);
	}
	FreeHashTbl(&hash);
    Zipper z = {NULL, chemin, final.dist}; 
	return z;
}