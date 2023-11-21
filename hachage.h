#ifndef __HACHAGE__
#define __HACHAGE__

#include "struct.h"
#include "boardManager.h"





int Hachage (const int w, Sommet s);

void InitHashTbl(HashTbl *h);
void ViderHashTbl(HashTbl* h);
void FreeHashTbl(HashTbl *h);
void AfficherHashTbl(HashTbl* h);
void AjouteHashTbl(Sommet cle, Sommet valeur, HashTbl* h);
Sommet ChercheHashTbl(HashTbl* h, Sommet cle);

void AjouteDescendants(Sommet a, HashTbl* h, bool horizontalWalls[BOARD_SIZE][BOARD_SIZE+1], bool verticalWalls[BOARD_SIZE][BOARD_SIZE+1]);



#endif //__HACHAGE__