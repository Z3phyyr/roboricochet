#include "hachagedist.h"
#include <limits.h>
#include <stdlib.h>



void FreeHashDist(HashDist* d) {
	for (int i=0; i<d->w; i++) {
		FreeSList(d->table[i]);
	}
	free(d->table);
	return;
}

void InitHashDist(HashDist* d) {
	d->table = malloc(d->w * sizeof(SList));
	assert (d->table != NULL);
	for (int k=0; k<d->w; k++) {
		d->table[k] = NULL;
	}
}


void AjouteHashDist(Sommet a, HashDist* d) {
	const int hache = Hachage(d->w, a);
	SList* l = d->table[hache];
	while (l != NULL) {
		if (EgaliteSommet(a, l->elem)) {
			l->elem.dist = a.dist;
			return;
		}
		l = l->suivant;
	}
	SList* new = malloc(sizeof(SList));
	assert (new != NULL);
	new->suivant = d->table[hache];
	CopieSommet(a, &new->elem);
	d->table[hache] = new;	
	return;
}


int RecupererDist(HashDist* d, Sommet a) {
	const int hache = Hachage(d->w, a);
	SList* l = d->table[hache];
	while (l != NULL) {
		if (EgaliteSommet(a, l->elem)) {
			return l->elem.dist;
		}
		l = l->suivant;
	}
	return INT_MAX;
}