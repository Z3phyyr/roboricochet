#ifndef _SDYNTAB_
#define _SDYNTAB_

#include "struct.h"
#include "graphe.h"

void InitPMinStack(SMinStack* stack);
void FreePMinStack(SMinStack* s);
void RedimensionnerPMinStack(SMinStack* s);
void InsererPMinStack(SMinStack* s, Sommet a, double p);
int ExtrairePMinStack(SMinStack* s, Sommet* v);

#endif //_SDYNTAB_