#include "minstackBis.h"
#include <stdlib.h>

void InitPMinStack(PMinStack* stack) {
	stack->capacite = 1;
	stack->remplissage = 0;
	stack->tableau = malloc(sizeof(Sommet));
    stack->priorite = malloc(sizeof(double));
    assert(stack->priorite != NULL);
	assert(stack->tableau != NULL);
}
void FreePMinStack(PMinStack* s) {
	if (s->tableau != NULL) { 
		free(s->tableau);
	}
    if (s->priorite != NULL) { 
		free(s->priorite);
	}
	s->remplissage = 0;
}

static int FilsGauche(int i) {
	return 2*i+1;
}
static int FilsDroit(int i) {
	return 2*i+2;
}
static int Pere(int i) {
	return (i-1)/2;
}

void EchangeSommet(Sommet* a, Sommet* b) {
	Sommet t;
	CopieSommet(*a, &t);
	CopieSommet(*b, a);
	CopieSommet(t, b);
}
void EchangeDouble(double* tab, int i, int j) {
    double temp = tab[i];
    tab[i] = tab[j];
    tab[j] = temp;
}

void RedimensionnerPMinStack(PMinStack* s) {
	int n = s->capacite;
	s->capacite = 2*n;
	s->remplissage = 0;

	Sommet* newtableau = malloc(s->capacite*sizeof(Sommet));
	assert(newtableau != NULL);
	double* newpriorite = malloc(s->capacite*sizeof(double));
	assert(newpriorite != NULL);
    double* oldpriorite = s->priorite;
	Sommet* oldtableau = s->tableau;

	s->tableau = newtableau;
    s->priorite = newpriorite;

	for (int i=0; i<n; i++) {
		InsererPMinStack(s, oldtableau[i], oldpriorite[i]);
	}
	free(oldpriorite);
	free(oldtableau);
}
void InsererPMinStack(PMinStack* s, Sommet a, double p) {
	if (s->remplissage >= s->capacite) {
		RedimensionnerPMinStack(s);
	}
	// insertion à la dernière case
	int i = s->remplissage;
	CopieSommet(a, &s->tableau[i]);
    s->priorite[i] = p;

    // percolation vers le haut
	int j = Pere(i);
	while (s->priorite[j] > s->priorite[i]) {
		EchangeSommet(&s->tableau[i], &s->tableau[j]);
        EchangeDouble(s->priorite,i,j);
		i = j;
		j = Pere(i);
	}
	s->remplissage++;
}




int ExtrairePMinStack(PMinStack* s, Sommet* v) {
	if (s->remplissage <= 0) {
		return -1;
	}

	CopieSommet(s->tableau[0], v);
	s->remplissage--;
	s->tableau[0] = s->tableau[s->remplissage];
    s->priorite[0] = s->priorite[s->remplissage];
	int i = 0;

	while (true) {//percolation vers le bas
		int i_max = i;
        int gauche = FilsGauche(i);
        int droit = FilsDroit(i);

		if (gauche < s->remplissage && 
            s->priorite[i_max] > s->priorite[gauche]) {
			i_max = gauche;
		}
		if (droit < s->remplissage && 
            s->priorite[i_max] > s->priorite[droit]) {
			i_max = droit;
		}
		if (i_max == i) break;

		EchangeSommet(&s->tableau[i], &s->tableau[i_max]);
        EchangeDouble(s->priorite, i, i_max);
		i = i_max;
	}
	return 0;
}