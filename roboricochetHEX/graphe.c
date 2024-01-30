#include "graphe.h"
#include "hachage.h"
#include <limits.h>
#include <stdlib.h>

//Modulo pour table de hachage
const long unsigned mersenne = 524287;

bool PasDeRobot(int q, int r, Sommet a, Couleur actuel) {
	for (int k=0; k<N_Robots; k++) {
		if (k != actuel) {
			if(q == a.positions[k].q && r == a.positions[k].r) return false;
		}
	}
	return true;
}

void CopieSommet(Sommet a, Sommet* b) {
	//Copie le sommet a dans le sommet b
	b->dist = a.dist;
	for (int k=0; k<N_Robots; k++) {
		b->positions[k] = a.positions[k];
	}
}

bool EgaliteSommet(Sommet a, Sommet b) {
	//Teste si a = b, c'est-à-dire si les positions des robot sont exactement les mêmes
	for (int k=0; k<N_Robots; k++) {
		if (a.positions[k].r != b.positions[k].r || a.positions[k].q != b.positions[k].q) {
			return false;
		}
	}
	return true;
}

Chemin* AjouterChemin(Chemin* c, Sommet a) {
	Chemin* new_maillon = malloc(sizeof(Chemin));
	assert(new_maillon != NULL);
	for (int k=0; k<N_Robots; k++) {
		new_maillon->positions[k] = a.positions[k]; 
	}
	new_maillon->suivant = c;
	return new_maillon;
}

SList* InsererSList(Sommet a, SList* l) {
	SList* tete = l;
	SList* precedent = NULL;
	while (l != NULL && a.dist > l->elem.dist) {
		precedent = l;
		l = l->suivant;
	}
	SList* new_maillon = malloc(sizeof(SList));
	assert(new_maillon != NULL);
	CopieSommet(a, &new_maillon->elem);

	if (precedent != NULL) {
		precedent->suivant = new_maillon;
	} else {
		tete = new_maillon;
	}
	new_maillon->suivant = l;
	return tete;
}

SList* ExtraireSList(SList* l, Sommet* dest) {
	if (l == NULL) {
		dest->dist = -1;
		return l;
	}
	CopieSommet(l->elem, dest);
	SList* suivant = l->suivant;
	free(l);
	return suivant;	
}

void FreeSList(SList* l) {
	while (l != NULL) {
		SList* suivant = l->suivant;
		free(l);
		l = suivant;
	}	
}

void AfficherSList(SList* c) {
	if (c == NULL) {
		return;
	}
	int q, r, s;
	for (int k=0; k<N_Robots; k++) {
		q = c->elem.positions[k].q;
		r = c->elem.positions[k].r;
		s = q + r - 9;
		printf("(%d, %d, %d) ", q, r, s);
	}
	printf("\n");
	AfficherSList(c->suivant);
}

void AfficherChemin(Chemin* c) {
	if (c == NULL) {
		return;
	}
	int q, r, s;
	for (int k=0; k<N_Robots; k++) {
		q = c->positions[k].q;
		r = c->positions[k].r;
		s = q + r - 9;
		printf("(%d, %d, %d) ", q, r, s);
	}
	printf("\n");
	AfficherChemin(c->suivant);
}

void FreeChemin (Chemin* c) {
	if (c == NULL) return;
	Chemin* suivant = c->suivant;
	free (c);
	FreeChemin(suivant);
}

void FreeZipper(Zipper* z) {
	FreeChemin(z->gauche);
	FreeChemin(z->droite);
	z->gauche = NULL;
	z->droite = NULL;
	z->distance_totale = 0;
}

Point DeplacementGauche(Sommet a, Couleur actuel, bool VerticalWalls[BOARD_SIZE+1][BOARD_SIZE + 1]) {
	/*
		ENTREE : coordonnées q, r, murs horizontaux
		SORTIE : coordonnées du point vers lequel un déplacement (vers le haut) est possible
	*/
	int q = a.positions[actuel].q;
	int r = a.positions[actuel].r;
	Point p;
	int wallq = q;
	while (!VerticalWalls[wallq][r] && PasDeRobot(wallq-1, r, a, actuel)) {
		wallq--;
	}
	if (wallq == q) {
		//Pas de mouvement
		p.q = -42;
		p.r = -42;
		return p;
	}
	p.q = wallq;
	p.r = r;
	return p;
}

Point DeplacementDroite (Sommet a, Couleur actuel, bool VerticalWalls[BOARD_SIZE+1][BOARD_SIZE + 1]) {
	int q = a.positions[actuel].q;
	int r = a.positions[actuel].r;
	Point p;
	int wallq = q;
	while (!VerticalWalls[wallq+1][r] && PasDeRobot(wallq+1, r, a, actuel)) {
		wallq++;
	}
	if (wallq == q) {
		//Pas de mouvement
		p.q = -42;
		p.r = -42;
		return p;
	}
	p.q = wallq;
	p.r = r;
	return p;
}

Point DeplacementNordEst (Sommet a, Couleur actuel, bool diagonalBasWalls[BOARD_SIZE + 1][BOARD_SIZE + 1]) {
	int q = a.positions[actuel].q;
	int r = a.positions[actuel].r;
	Point p;
	int wallq = q;
	int wallr = r;
	while (!diagonalBasWalls[wallq+1][wallr] && PasDeRobot(wallq+1, wallr-1, a, actuel)) {
		wallr--; 
		wallq++;
	}
	if ((wallr == r) && (wallq == q)) {
		//Pas de mouvement
		p.q = -42;
		p.r = -42;
		return p;
	}
	p.q = wallq;
	p.r = wallr;
	return p;
}

Point DeplacementSudOuest (Sommet a, Couleur actuel, bool diagonalBasWalls[BOARD_SIZE + 1][BOARD_SIZE + 1]) {
	int q = a.positions[actuel].q;
	int r = a.positions[actuel].r;
	Point p;
	int wallq = q;
	int wallr = r;
	while (!diagonalBasWalls[wallq][wallr+1] && PasDeRobot(wallq-1, wallr+1, a, actuel)) {
		wallr++; 
		wallq--;
	}
	if ((wallr == r) && (wallq == q)) {
		//Pas de mouvement
		p.q = -42;
		p.r = -42;
		return p;
	}
	p.q = wallq;
	p.r = wallr;
	return p;
}

Point DeplacementSudEst (Sommet a, Couleur actuel, bool diagonalHautWalls[BOARD_SIZE + 1][BOARD_SIZE+1]) {
	int q = a.positions[actuel].q;
	int r = a.positions[actuel].r;
	Point p;
	int wallr = r;
	while (!diagonalHautWalls[q][wallr+1] && PasDeRobot(q, wallr+1, a, actuel)) {
		wallr++;
	}
	if (wallr == r) {
		//Pas de mouvement
		p.q = -42;
		p.r = -42;
		return p;
	}
	p.q = q;
	p.r = wallr;
	return p;
}

Point DeplacementNordOuest (Sommet a, Couleur actuel, bool diagonalHautWalls[BOARD_SIZE + 1][BOARD_SIZE+1]) {
	int q = a.positions[actuel].q;
	int r = a.positions[actuel].r;
	Point p;
	int wallr = r;
	while (!diagonalHautWalls[q][wallr] && PasDeRobot(q, wallr-1, a, actuel)) {
		wallr--;
	}
	if (wallr == r) {
		//Pas de mouvement
		p.q = -42;
		p.r = -42;
		return p;
	}
	p.q = q;
	p.r = wallr;
	return p;
}

SList* DescendantsDirects (Sommet a, bool VerticalWalls[BOARD_SIZE+1][BOARD_SIZE+1], 
									 bool diagonalHautWalls[BOARD_SIZE + 1][BOARD_SIZE+1],
									 bool diagonalBasWalls[BOARD_SIZE + 1][BOARD_SIZE + 1]) {
	//Recupere les descendants directs d'un sommet a
	
	SList* l = NULL;
	for (int k=0; k<N_Robots; k++) {
		for (int d=0; d<6; d++) {
			//Gauche 0 ; Haut-Gauche 1 ; Haut-Droite 2 ; Droite 3 ; Bas-Droite 4 ; Bas-Gauche 5
			Sommet b;
			CopieSommet(a, &b);
			Point p = {0, 0};
			switch (d) {
				case 0:
					// r negatif
					p = DeplacementGauche(a, k, VerticalWalls);
					break;
				case 1:
					// q négatif et r positif
					p = DeplacementNordOuest(a, k, diagonalHautWalls);
					break;
				case 2:
					// q positif
					p = DeplacementNordEst(a, k, diagonalBasWalls);
					break;
				case 3:
					// r positif
					p = DeplacementDroite(a, k, VerticalWalls);
					break;
				case 4:
					// q positif et r négatif
					p = DeplacementSudEst(a, k, diagonalHautWalls);
					break;
				case 5:
					// q négatif
					p = DeplacementSudOuest(a, k, diagonalBasWalls);
					break;
			}
			if (p.r != -42) {
				//Présence de mouvement
				b.positions[k] = p;
				l = InsererSList(b, l);
			}	
		}
	}
	return l;
}

Zipper Dijkstra(Sommet s, Point t, Couleur couleur, 
				 bool VerticalWalls[BOARD_SIZE+1][BOARD_SIZE+1], 
				 bool DiagupWalls[BOARD_SIZE + 1][BOARD_SIZE+1],
				 bool DiagDownWalls[BOARD_SIZE + 1][BOARD_SIZE + 1]) {
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
	FreeHashDist(&d);
	
	printf("Distance finale = %d\n", final.dist);
	printf("Taille finale des tables de hachage : w = %lu; size = %lu\n\n", h.w, h.size);
	//t non accessible (ne devrait pas arriver en pratique)
	if (final.dist == -1) return (Zipper) {NULL, NULL, 0};
	
	//Construction du chemin à l'aide de h
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
		courant = ChercheHashTbl(&h, courant);
	}
	FreeHashTbl(&h);
	Zipper z = {NULL, chemin, final.dist};
	
	return z;
}