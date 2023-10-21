#include "graphe.h"
#include "hachage.h"
#include <limits.h>
#include <stdlib.h>

//Modulo pour table de hachage
const long unsigned mersenne = 131071;

bool PasDeRobot(int i, int j, Sommet a, Couleur actuel) {
	for (int k=0; k<N_Robots; k++) {
		if (k != actuel) {
			if(i == a.positions[k].i && j == a.positions[k].j) return false;
		}
	}
	return true;
}

Point DeplacementHaut (Sommet a, Couleur actuel, bool horizontalWalls[BOARD_SIZE][BOARD_SIZE + 1]) {
	/*
		ENTREE : coordonnées i, j, murs verticaux
		SORTIE : coordonnées du point vers lequel un déplacement (vers le haut) est possible
	*/
	int i = a.positions[actuel].i;
	int j = a.positions[actuel].j;
	Point p;
	int wallj = j;
	while (!horizontalWalls[i][wallj] && PasDeRobot(i, wallj-1, a, actuel)) {
		wallj--;
	}
	if (wallj == j) {
		//Pas de mouvement
		p.i = -42;
		p.j = -42;
		return p;
	}
	p.i = i;
	p.j = wallj;
	return p;
}

Point DeplacementDroite(Sommet a, Couleur actuel, bool verticalWalls[BOARD_SIZE][BOARD_SIZE + 1]) {
	int i = a.positions[actuel].i;
	int j = a.positions[actuel].j;
	Point p;
	int walli = i+1;
	while (!verticalWalls[j][walli] && PasDeRobot(walli, j, a, actuel)) {
		walli++;
	}
	if (walli == i+1) {
		//Pas de mouvement
		p.i = -42;
		p.j = -42;
		return p;
	}
	p.j = j;
	p.i = walli - 1;
	return p;
}

Point DeplacementBas(Sommet a, Couleur actuel, bool horizontalWalls[BOARD_SIZE][BOARD_SIZE + 1]) {
	int i = a.positions[actuel].i;
	int j = a.positions[actuel].j;
	Point p;
	int wallj = j+1;
	while (!horizontalWalls[i][wallj] && PasDeRobot(i, wallj, a, actuel)) {
		wallj++;
	}
	if (wallj == j+1) {
		//Pas de mouvement
		p.i = -42;
		p.j = -42;
		return p;
	}
	p.i = i;
	p.j = wallj - 1;
	return p;
}

Point DeplacementGauche(Sommet a, Couleur actuel, bool verticalWalls[BOARD_SIZE][BOARD_SIZE + 1]) {
	int i = a.positions[actuel].i;
	int j = a.positions[actuel].j;
	Point p;
	int walli = i;
	while (!verticalWalls[j][walli] && PasDeRobot(walli - 1, j, a, actuel)) {
		walli--;
	}
	if (walli == i) {
		//Pas de mouvement
		p.i = -42;
		p.j = -42;
		return p;
	}
	p.j = j;
	p.i = walli;
	return p;
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
		if (a.positions[k].i != b.positions[k].i || a.positions[k].j != b.positions[k].j) {
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

SList* DescendantsDirects (Sommet a, bool horizontalWalls[BOARD_SIZE][BOARD_SIZE+1], bool verticalWalls[BOARD_SIZE][BOARD_SIZE+1]) {
	//Recupere les descendants directs d'un sommet a
	
	SList* l = NULL;
	for (int k=0; k<N_Robots; k++) {
		for (int d=0; d<4; d++) {
			//NORD 0 ; EST 1 ; SUD 2 ; OUEST 3
			Sommet b;
			CopieSommet(a, &b);
			Point p = {0, 0};
			switch (d) {
				case 0:
					//j négatif
					p = DeplacementHaut(a, k, horizontalWalls);
					break;
				case 1:
					//i positif
					p = DeplacementDroite(a, k, verticalWalls);
					break;
				case 2:
					//j positif
					p = DeplacementBas(a, k, horizontalWalls);
					break;
				case 3:
					//i négatif
					p = DeplacementGauche(a, k, verticalWalls);
					break;
			}
			if (p.i != -42) {
				//Présence de mouvement
				b.positions[k] = p;
				l = InsererSList(b, l);
			}	
		}
	}
	return l;
}

void AfficherSList(SList* c) {
	if (c == NULL) {
		return;
	}
	for (int k=0; k<N_Robots; k++) {
		printf("(%d, %d)  ", c->elem.positions[k].i, c->elem.positions[k].j);
	}
	printf("\n");
	AfficherSList(c->suivant);
}

void AfficherChemin(Chemin* c) {
	if (c == NULL) {
		return;
	}
	for (int k=0; k<N_Robots; k++) {
		printf("(%d, %d)  ", c->positions[k].i, c->positions[k].j);
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

void FreeZipper(Zipper z) {
	FreeChemin(z.gauche);
	FreeChemin(z.droite);
}


Zipper Dijkstra(Sommet s, Point t, Couleur couleur, 
				 bool horizontalWalls[BOARD_SIZE][BOARD_SIZE+1], 
				 bool verticalWalls[BOARD_SIZE][BOARD_SIZE+1]) {
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
	InsererStack(&file, s);
	s.dist = 0;
	Sommet final;
	final.dist = -1;
	Sommet v;
	Sommet w;
	while (file.remplissage != 0) {
		ExtraireMinStack(&file, &v);
		if (tours % 1000 == 0) printf("(%d, %d) ; dist : %d ; tours : %d\n", v.positions[couleur].i, v.positions[couleur].j, v.dist, tours);
				
		if (v.positions[couleur].i == t.i && v.positions[couleur].j == t.j) {
			CopieSommet(v, &final);
			FreeSMinStack(&file);
		} else {
			SList* descendants = DescendantsDirects(v, horizontalWalls, verticalWalls);
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
	for (int k=0; k<N_Robots; k++) {
		printf("(%d, %d)  ", final.positions[k].i, final.positions[k].j);
	}
	printf("\nDistance finale = %d\n", final.dist);
	//t non accessible (ne devrait pas arriver en pratique)
	if (final.dist == -1) return (Zipper) {NULL, NULL};
	
	//Construction du chemin à l'aide de h
	Chemin* chemin = AjouterChemin(NULL, final);
	Sommet courant;
	CopieSommet(final, &courant);
	while (!EgaliteSommet(courant, s)) {
		courant = ChercheHashTbl(&h, courant);
		if (courant.dist == -1) {
			//Echec de récupération du sommet
			printf("Echec de récupération d'un sommet\n\n");
			return (Zipper){NULL, NULL};
		} else {
			chemin = AjouterChemin(chemin, courant);
		}
	}
	FreeHashTbl(&h);
	Zipper z = {NULL, chemin};
	return z;
}