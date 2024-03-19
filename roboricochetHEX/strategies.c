#include "strategies.h"


Zipper DijkstraLimiteAutresRobots(Sommet s, Point t, Couleur couleur, const unsigned CoupsMaxAutresRobots,
				 bool VerticalWalls[BOARD_SIZE+1][BOARD_SIZE+1], 
				 bool DiagupWalls[BOARD_SIZE + 1][BOARD_SIZE+1],
				 bool DiagDownWalls[BOARD_SIZE + 1][BOARD_SIZE + 1]) {
	
	const long unsigned mersenne = 524287;

	/*
		ENTREE : sommets s et t, tableaux pour obtenir les descendants
		SORTIE : chemin le plus court de s à t
	*/
	int tours = 0;
	HashTbl h = {NULL, mersenne};
	HashDist d = {NULL, mersenne};
	InitHashTbl(&h);
	InitHashDist(&d);
	SMinStack file;
	InitSMinStack(&file);
	s.dist = 0;
	InsererStack(&file, s);
	Sommet final;
	final.dist = -1;
	Sommet v;
	Sommet w;
	SList* descendants = NULL;
	
	while (file.remplissage != 0) {
		ExtraireMinStack(&file, &v);
		if (tours % 100000 == 0) printf("dist : %d ; tours : %d\n", v.dist, tours);
				
		if (v.positions[couleur].q == t.q && v.positions[couleur].r == t.r) {
			printf("Nombre total de tours : %d\n", tours);
			CopieSommet(v, &final);
			FreeSMinStack(&file);
		} else {
			descendants = DescendantsDirects(v, VerticalWalls, DiagupWalls, DiagDownWalls);
			while (descendants != NULL) {
				descendants = ExtraireSList(descendants, &w);
				int newdist = RecupererDist(&d, w);
				if (newdist > v.dist + 1) {
					w.dist = v.dist + 1;
					AjouteHashTbl(w, v, &h);
					AjouteHashDist(w, &d);
					InsererStack(&file, w);
				}
			}
			FreeSList(descendants);
		}
		tours++;
	}

	Zipper z = {NULL, NULL, 0};
	return z;
}