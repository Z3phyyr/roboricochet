#ifndef COMPARAISON
#define COMPARAISON

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#include "graphe.h"
#include "astar.h"
#include "struct.h"
#include "boardManager.h"

#include "../include/SDL.h"
#include "../include/SDL_ttf.h"

int comparaison (GameStruct* g, Donnees* d, int n);
void AfficherDonnees (Donnees* d, int n);

int recuperer(GameStruct* g, char* file);

#endif //COMPARAISON