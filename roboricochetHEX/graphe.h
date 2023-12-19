#ifndef __GRAPHE__
#define __GRAPHE__

#include "boardManager.h"
#include "hachagedist.h"
#include "minstack.h"
#include <stdbool.h>
#include <stdlib.h>

void CopieSommet(Sommet a, Sommet* b);
bool EgaliteSommet(Sommet a, Sommet b);

void AfficherSList(SList* c);
SList* InsererSList(Sommet a, SList* l);
SList* ExtraireSList(SList* l, Sommet* dest);
void FreeSList(SList* l);

Chemin* AjouterChemin(Chemin* c, Sommet a);
void FreeChemin (Chemin* c);

void FreeZipper(Zipper z);

SList* DescendantsDirects (Sommet a, bool horizontalWalls[BOARD_SIZE+1][BOARD_SIZE+1], 
									 bool diagonalHautWalls[BOARD_SIZE + 1][BOARD_SIZE+1],
									 bool diagonalBasWalls[BOARD_SIZE + 1][BOARD_SIZE + 1]);

Zipper Dijkstra(Sommet s, Point t, Couleur couleur,
				 bool horizontalWalls[BOARD_SIZE+1][BOARD_SIZE+1], 
				 bool diagonalHautWalls[BOARD_SIZE + 1][BOARD_SIZE+1],
				 bool diagonalBasWalls[BOARD_SIZE + 1][BOARD_SIZE + 1]);


#endif //__GRAPHE__