#ifndef _SDYNTAB_
#define _SDYNTAB_

#include "struct.h"
#include "graphe.h"

void InitSMinStack(SMinStack* stack);
void FreeSMinStack(SMinStack* s);
void RedimensionnerStack(SMinStack* s);
void InsererStack(SMinStack* s, Sommet a);
int ExtraireMinStack(SMinStack* s, Sommet* v);

#endif //_SDYNTAB_