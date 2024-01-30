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


int GetButtonTextTexture(SDL_Renderer* renderer, Button* button, const char* texte, int size, const char* font_path);
void FreeButton(Button* b);

int AfficherBouton(SDL_Renderer* renderer, Button* start);
bool CheckBoutonPresse(Button* b, int x, int y);

int AffichageTexteInformatif(SDL_Renderer* renderer, int distance_totale);
int AffichagePosition(SDL_Renderer* renderer, int distance, int distance_totale);
#endif //INTERFACE