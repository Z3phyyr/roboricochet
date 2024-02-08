#ifndef _PRIOMINSTACK_
#define _PRIOMINSTACK_

#include "struct.h"
#include "graphe.h"


typedef struct PrioMinStack {
    int capacite;
    int remplissage;
    Sommet* tableau;
    double* priorite;
} PMinStack;

void InitPMinStack(PMinStack* stack);
void FreePMinStack(PMinStack* s);
void RedimensionnerPMinStack(PMinStack* s);
void InsererPMinStack(PMinStack* s, Sommet a, double p);
int ExtrairePMinStack(PMinStack* s, Sommet* v);

#endif //_PRIOMINSTACK_