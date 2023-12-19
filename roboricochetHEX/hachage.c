#include "hachage.h"
#include "graphe.h"
#include <stdlib.h>

void FreeCList(CList* l) {
	CList* suivant = NULL;
	while (l != NULL) {
		suivant = l->suivant;
		free(l);
		l = suivant;
	}
	return;
}

int Hachage(const int w, Sommet s) {
  	// Fonction de hachage selon Centrale
  	long int N = BOARD_SIZE;
  	int hache = 0;
  	for (int k = 0; k < N_Robots; k++) {
    	hache = (hache + ((s.positions[k].q + BOARD_SIZE * s.positions[k].r) * N)) % w;
    	N = N * BOARD_SIZE * BOARD_SIZE;
  	}
  	return hache;
}

void InitHashTbl(HashTbl *h) {
	h->table = malloc(h->w * sizeof(CList*));
	assert(h->table != NULL);
	for (int k=0; k<h->w; k++) {
		h->table[k] = NULL;
	}
}

void FreeHashTbl(HashTbl *h) {
  for (int i = 0; i < h->w; i++) {
    FreeCList(h->table[i]);
  }
  free(h->table);
}

void AjouteHashTbl(Sommet cle, Sommet valeur, HashTbl *h) {
  	/*
		Ajoute sans doublons le couple (cle, valeur) dans h
	*/
	const int hache = Hachage(h->w, cle);
	CList* l = h->table[hache];
	while (l != NULL) {
		if (EgaliteSommet(cle, l->c.cle)) {
			CopieSommet(valeur, &l->c.valeur);
			return;
		}
		l = l->suivant;
	}
  	CList *new_l = malloc(sizeof(CList));
	assert(new_l != NULL);
  
  	CopieSommet(cle, &new_l->c.cle);
	CopieSommet(valeur, &new_l->c.valeur);
  	new_l->suivant = h->table[hache];
  	h->table[hache] = new_l;
}

Sommet ChercheHashTbl(HashTbl* h, Sommet cle) {
	/*
		Cherche, supprime et renvoie la valeur associée à cle si celle-ci existe, sinon renvoie un sommet de dist -1
	*/
	const int hache = Hachage(h->w, cle);
	CList* l = h->table[hache];
	while (l != NULL) {
		if (EgaliteSommet(l->c.cle, cle)) {
			return l->c.valeur;
		}
		l = l->suivant;
	}
	Sommet a = {{}, -1};
	return a;
}
