#ifndef __GRAPHE__
#define __GRAPHE__

#include "boardManager.h"
#include "hachagedist.h"
#include "minstack.h"
#include <stdbool.h>
#include <stdlib.h>


void AfficherSList(SList* c);

void CopieSommet(Sommet a, Sommet* b);
bool EgaliteSommet(Sommet a, Sommet b);

SList* InsererSList(Sommet a, SList* l);
SList* ExtraireSList(SList* l, Sommet* dest);
void FreeSList(SList* l);
SList* DescendantsDirects (Sommet a, bool horizontalWalls[BOARD_SIZE][BOARD_SIZE+1], bool verticalWalls[BOARD_SIZE][BOARD_SIZE+1]);
Chemin* AjouterChemin(Chemin* c, Sommet a);
void AfficherChemin(Chemin* c);
void FreeChemin (Chemin* c);

void FreeZipper(Zipper z);



Zipper Dijkstra(Sommet s, Point t, Couleur couleur,
				 bool horizontalWalls[BOARD_SIZE][BOARD_SIZE+1], 
				 bool verticalWalls[BOARD_SIZE][BOARD_SIZE+1]);


#endif //__GRAPHE__