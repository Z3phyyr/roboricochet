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


void RedimmensionerHashDist(HashDist* d) {
	SList** old = d->table;
	int old_w = d->w;
	d->w = 2*old_w;
	d->table = (SList**) malloc(d->w*sizeof(CList*));
	assert(d->table != NULL);
	d->size = 0;
	SList* l = NULL;
	SList* suivant = NULL;
	for(int i=0; i<old_w; i++) {
		l = old[i];
		d->table[i] = NULL;
		while (l != NULL) {
			AjouteHashDist(l->elem, d);
			suivant = l->suivant;
			free(l);
			l = suivant;
		}
	}
	free(old);
}


void AjouteHashDist(Sommet a, HashDist* d) {
	
	if (d->w < d->size) {
		RedimmensionerHashDist(d);
	}

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
	d->size++;
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