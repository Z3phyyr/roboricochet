#include "minstack.h"
#include <stdlib.h>

void InitSMinStack(SMinStack* stack) {
	stack->capacite = 1;
	stack->remplissage = 0;
	stack->tableau = malloc(sizeof(Sommet));
	assert(stack->tableau != NULL);
}

void FreeSMinStack(SMinStack* s) {
	if (s->tableau != NULL) { 
		free(s->tableau);
	}
	s->remplissage = 0;
}

int FilsGauche(int i) {
	return 2*i+1;
}

int FilsDroit(int i) {
	return 2*i+2;
}

int Pere(int i) {
	return (i-1)/2;
}

void echange(Sommet* a, Sommet* b) {
	Sommet t;
	CopieSommet(*a, &t);
	CopieSommet(*b, a);
	CopieSommet(t, b);
}

void RedimensionnerStack(SMinStack* s) {
	int n = s->capacite;
	s->capacite = 2*n;
	Sommet* newtableau = malloc(s->capacite*sizeof(Sommet));
	assert(newtableau != NULL);
	Sommet* old = s->tableau;
	s->tableau = newtableau;
	s->remplissage = 0;
	for (int i=0; i<n; i++) {
		InsererStack(s, old[i]);
	}
	free(old);
}

void InsererStack(SMinStack* s, Sommet a) {
	if (s->remplissage >= s->capacite) {
		RedimensionnerStack(s);
	}
	int i = s->remplissage;
	CopieSommet(a, &s->tableau[i]);
	int j = Pere(i);
	while (s->tableau[j].dist > s->tableau[i].dist) {
		echange(&s->tableau[i], &s->tableau[j]);
		i = j;
		j = Pere(i);
	}
	s->remplissage++;
}

int ExtraireMinStack(SMinStack* s, Sommet* v) {
	if (s->remplissage <= 0) {
		return -1;
	}
	CopieSommet(s->tableau[0], v);
	s->remplissage--;
	s->tableau[0] = s->tableau[s->remplissage];
	int i = 0;
	while (true) {
		int i_max = i, gauche = FilsGauche(i), droit = FilsDroit(i);
		if (gauche < s->remplissage && s->tableau[i_max].dist > s->tableau[gauche].dist) {
			i_max = gauche;
		}
		if (droit < s->remplissage && s->tableau[i_max].dist > s->tableau[droit].dist) {
			i_max = droit;
		}
		if (i_max == i) break;

		echange(&s->tableau[i], &s->tableau[i_max]);
		i = i_max;
	}
	return 0;
}