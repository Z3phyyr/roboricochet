#include "graphe.h"
#include "astar.h"
#include "hachage.h"
#include "minStackBis.h"
#include <limits.h>
#include <stdlib.h>
#include <math.h>

// heuristiques

double h_euclidienne (Sommet a, Point final){
    double max = sqrt(2.)*16;
    double dist_eucl = sqrt(pow(fabs(final.i-a.position[couleur].i),2) + pow(fabs(final.j-a.position[couleur].j),2));
    return dist_eucl/max;
}


// création du chemin solution
Chemin* FindPath (Chemin* chemin, Sommet t, Sommet s, int* total_dist){
    int d = t.dist;
    if(d==0 && EgaliteSommet(final, courant)){
        *total_dist +=1;
        return AjouterChemin(chemin, t);
    }
    descendants = DescendantDirects (t, horizontalWalls, verticalWalls);
    Sommet u;
    while (descendants != NULL) {
		descendants = ExtraireSList(descendants, &u);
        if (d == u.dist + 1){
            *total_dist +=1;
            return FindPath(dist, AjouterChemin(chemin,u), u, final, total_dist);
        }
    }
    //Echec de récupération du sommet
	printf("Echec de récupération d'un sommet\n\n");
    *total_dist = 0;
	return NULL
}

//fonction principale
Zipper a_star (Sommet s, Point t, Couleur couleur, double (*h) (Sommet,Point) /*fonction heuristique*/,
				 bool horizontalWalls[BOARD_SIZE][BOARD_SIZE+1], 
				 bool verticalWalls[BOARD_SIZE][BOARD_SIZE+1]) {
   // création de la file avec s de priorité h(s)
    s.dist = 0;
    PMinStack file;
    InitPMinStack(&file);
    double p = (*h) (s,t);
    InsererStack(&file,s,p);

   // création de variables utiles
    Sommet v;
    Sommet w;
    SList* descendants = NULL;

    while(file.remplissage != 0){
		//if (tours % 1000 == 0) printf("(%d, %d) ; dist : %d ; tours : %d\n", v.positions[couleur].i, v.positions[couleur].j, v.dist, tours);

        ExtrairePMinStack(&file,&v);
        if (v.positions[couleur].i == t.i && v.positions[couleur].j == t.j){

            int total_dist;
            Chemin* res = NULL;
            res = FindPath (chemin, t, s, &total_dist)
            Zipper z = {NULL, res, *total_dist};

            free(total_dist);
            FreeSList(descendants);

            return rep;
        }
        
        else {
			descendants = DescendantsDirects(v, horizontalWalls, verticalWalls);
            while (descendants != NULL) {
   				descendants = ExtraireSList(descendants, &w);
                if (w.dist > v.dist + 1){
					w.dist = v.dist + 1;
                    p = (*h) (w,t);
                    InsererPMinStack(&file,&w,p);
                }
            }
        }
    }
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