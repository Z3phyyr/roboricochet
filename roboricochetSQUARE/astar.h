#ifndef __ASTAR__
#define __ASTAR__

#include "boardManager.h"
#include "hachagedist.h"
#include "minstack.h"
#include <stdbool.h>
#include <stdlib.h>

double h_euclidienne (Sommet a, Point final);

Zipper a_star(Sommet s, Point t, Couleur couleur, int (*h) (Sommet),
				 bool horizontalWalls[BOARD_SIZE][BOARD_SIZE+1], 
				 bool verticalWalls[BOARD_SIZE][BOARD_SIZE+1]);

#endif //__ASTAR__