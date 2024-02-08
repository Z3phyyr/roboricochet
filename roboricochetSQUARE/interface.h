#ifndef INTERFACE
#define INTERFACE

#include "../include/SDL.h"
#include "../include/SDL_ttf.h"

#include <assert.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "textureManager.h"
#include "struct.h"


int SetButtonTextTexture(SDL_Renderer* renderer, Button* button, const char* texte, 
						 int size, const char* font_path, SDL_Color c);

int AfficherBouton(SDL_Renderer* renderer, Button* start);
bool CheckBoutonPresse(Button* b, int x, int y);

int AffichageTexteInformatif(SDL_Renderer* renderer, int distance_totale);
int AffichagePosition(SDL_Renderer* renderer, int distance, int distance_totale);


Button* InitBoutonsHeuristiques(SDL_Renderer* renderer);
void FreeBoutonsHeuristiques(Button* b);
int AfficherChoixHeuristiques (SDL_Renderer* renderer, Button* heuristiques);
int AffichageHeuristiqueChoisie(SDL_Renderer* renderer, int choixH);


Button* InitBoutonsJetons(SDL_Renderer* renderer, dimTexture* jetons);
void FreeBoutonsJetons(Button* b);
int AfficherChoixJetons(SDL_Renderer* renderer, Button* jetons);
int AffichageJetonChoisi(SDL_Renderer* renderer, int choixJ);


#endif //INTERFACE