#ifndef _HACHAGEDIST_
#define _HACHAGEDIST_

#include "struct.h"
#include "hachage.h"
#include "graphe.h"






void FreeHashDist(HashDist* d);
void InitHashDist(HashDist* d);
void AjouteHashDist(Sommet a, HashDist* d);
int RecupererDist (HashDist* d, Sommet a);



#endif //_HACHAGEDIST_