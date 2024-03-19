#ifndef __ASTAR__
#define __ASTAR__

#include "boardManager.h"
#include "hachage.h"
#include "minStackBis.h"
#include <limits.h>
#include <stdlib.h>

const static long unsigned mersenne = 524287;

double h_euclidienne (Sommet a, Point final, Couleur c);
double h_manhattan (Sommet a, Point final, Couleur c);
double h_rapide (Sommet a, Point final, Couleur c);

Zipper a_star (Sommet s, Point t, Couleur couleur, double (*h) (Sommet ,Point , Couleur) /*fonction heuristique*/,
				 bool VerticalWalls[BOARD_SIZE+1][BOARD_SIZE+1], 
				 bool DiagupWalls[BOARD_SIZE + 1][BOARD_SIZE+1],
				 bool DiagDownWalls[BOARD_SIZE + 1][BOARD_SIZE + 1]);

#endif //__ASTAR__