#ifndef __HACHAGE__
#define __HACHAGE__

#include "struct.h"
#include "boardManager.h"

void FreeCList(CList* l)

int Hachage (const int w, Sommet s);

void InitHashTbl(HashTbl *h);

void FreeHashTbl(HashTbl *h);

void AjouteHashTbl(Sommet cle, Sommet valeur, HashTbl* h);

Sommet ChercheHashTbl(HashTbl* h, Sommet cle);


#endif //__HACHAGE__