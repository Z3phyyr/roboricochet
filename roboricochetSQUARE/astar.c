#include "astar.h"
#include <math.h>

//heuristiques

double h_euclidienne (Sommet a, Point final, Couleur c) {
    double max = sqrt(2.)*BOARD_SIZE;
    double dist_eucl = sqrt(pow(abs(final.i - a.positions[c].i),2) + 
                            pow(abs(final.j - a.positions[c].j),2));
    return dist_eucl/max;
}

double h_manhattan (Sommet a, Point final, Couleur c) {
    double max = BOARD_SIZE*2;
    double dist_man = abs(final.i - a.positions[c].i) + abs(final.j - a.positions[c].j);
    return dist_man / max;
}

double h_3 (Sommet a, Point final, Couleur c) {
    double max = BOARD_SIZE*2*N_Robots;
    double dist_man = 0;
    for(int k=0; k<N_Robots; k++) {
        dist_man += abs(final.i - a.positions[k].i) + abs(final.j - a.positions[k].j);
    }
    return dist_man / max;
}




//fonction principale
Zipper a_star (Sommet s, Point t, Couleur couleur, double (*h) (Sommet ,Point , Couleur) /*fonction heuristique*/,
				 bool horizontalWalls[BOARD_SIZE][BOARD_SIZE+1], 
				 bool verticalWalls[BOARD_SIZE][BOARD_SIZE+1]) {
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
       
        if (v.positions[couleur].i == t.i && v.positions[couleur].j == t.j){
            printf("Nombre total de tours : %d\n", tours);
            FreePMinStack(&file);
            CopieSommet(v, &final);
        } else {
       	    descendants = DescendantsDirects(v, horizontalWalls, verticalWalls);
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


/************** EXEMPLE DE FONTION AVEC UNE FONCTION EN ARGUMENT ***********************

double carre (double x) { 
    return x*x;
}

double integrale(double b_inf, double b_sup, int pas, double (*f)(double)){
    double surface = 0.;
    double h;
    h = (b_sup - b_inf)/pas;
    for(int i=0; i< pas; i++){
        surface += h * (*f) (b_inf+i*h);
    }
    return surface;
}

void main(){
   double b_inf, b_sup, aire;
   int pas;
   b_inf = 1., b_sup = 6., pas = 2000;
   aire = integrale (b_inf, b_sup, pas, carre);
   printf("Aire : %f\n", aire);
}

*/