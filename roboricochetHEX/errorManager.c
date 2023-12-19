#include "errorManager.h"
#include <stdio.h>



void SDL_ExitWithError (const char* msg) {
	SDL_Log("ERREUR : %s > %s\n", msg, SDL_GetError());
	TTF_Quit();
	SDL_Quit();
	exit (1);
}

void DestroyEverything (GameStruct* g) {
	DestroyDimTexture(g->jetons);
	DestroyDimTexture(g->robots);
	SDL_DestroyRenderer(g->renderer);
	SDL_DestroyWindow(g->window);
}

void throwwithCondition (bool condition, const char* msg, GameStruct* g) {
	if (condition) {
		DestroyEverything(g);
		SDL_ExitWithError(msg);
	}
}