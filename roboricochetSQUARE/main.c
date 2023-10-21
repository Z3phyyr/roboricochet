#include "../include/SDL.h"
#include "../include/SDL_ttf.h"

#include <assert.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "graphe.h"
#include "errorManager.h"
#include "boardManager.h"
#include "textureManager.h"

/*********************TODO****************************/
/*
	- Changer SList en SDynTab
	- Réfléchir à une heuristique sur les autres robots (i.e. limiter leurs déplacements)
	- Passer sur la version hexagonale (rip)
*/
/*****************************************************/


#define FPSMILLISECONDLIMIT 33

const int window_w = 1200;
const int window_h = 800;




int main (int argc, char** argv) {
	GameStruct g;

	if (SDL_Init(SDL_INIT_VIDEO) != 0 || TTF_Init() != 0) {
		SDL_ExitWithError("Error during Initialisation");
	}

	g.window = SDL_CreateWindow("RoboRicochet Square", 150, 25, window_w, window_h, 0);
	if (g.window == NULL) {
		SDL_ExitWithError("Error during Window Creation");
	}

	g.renderer = SDL_CreateRenderer(g.window, -1, SDL_RENDERER_SOFTWARE);
	if (g.renderer == NULL) {
		SDL_DestroyWindow(g.window);
		SDL_ExitWithError("Error during renderer creation");
	}
	srand (time(NULL));

	/******************************************/
	bool is_running = true;
	bool cheminpresent = false;
	char* init_str = "r";
	if (BOARD_SIZE != 16) {
		strcpy(init_str, "y");
	}

	g.jetons = CreateDimTextureFromImage(g.renderer, "C:/Program_Files/OCaml64/home/Admin/C/SDL/roboricochetSQUARE/jetons.bmp", 1, 100, 100);
	g.robots = CreateDimTextureFromImage(g.renderer, "C:/Program_Files/OCaml64/home/Admin/C/SDL/roboricochetSQUARE/robots.bmp", 1, 100, 100);


	g.actuel = (Sommet) {{{1, 2}, {0, 0}, {7, 4}, {12, 5}}, 0};
	Zipper z = {NULL, NULL};


	/******************************************/
	throwwithCondition(InitializeRoboRicochet(&g, "yousk2") < 0, "Error during Game Initialisation", &g);
	
	

	
	
	while (is_running) {
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
				case SDL_QUIT:
					is_running = false;
					break;
				case SDL_KEYDOWN: 
					switch (e.key.keysym.sym) {
						case SDLK_ESCAPE:
							is_running = false;
							break;
						case SDLK_d:
							throwwithCondition(InitializeRoboRicochet(&g, "z") < 0, "Error during initialisation", &g);
						case SDLK_r:
							throwwithCondition(InitializeRoboRicochet(&g, "r") < 0, "Error during initialisation", &g);
							break;
						case SDLK_c:
							FreeZipper(z);
							printf("Lancement de DIJKSTRA\n");
							z = Dijkstra(g.actuel, (Point){9, 3}, BLUE, g.horizontalWalls, g.verticalWalls);
							AfficherChemin(z.droite);
							cheminpresent = true;
							printf("");
							break;


						case SDLK_LEFT:
							if (cheminpresent) {
								throwwithCondition(DeplaceGauche(g.renderer, g.robots, &g.actuel, g.board, &z) != 0, "DeplaceGauche triggered an exception", &g);
							}
							break;
						case SDLK_RIGHT:
							if (cheminpresent) {
								throwwithCondition(DeplaceDroite(g.renderer, g.robots, &g.actuel, g.board, &z) != 0, "DeplaceDroite triggered an exception", &g);
							}
							
							break;


						case SDLK_f:
							FreeZipper(z);
							break;

						default:break;
					}
					break;
				
				default:break;
			}
		}
		SDL_Delay(FPSMILLISECONDLIMIT);
		SDL_RenderPresent(g.renderer);
	}

	DestroyEverything (&g);
	FreeZipper(z);
	TTF_Quit();
	SDL_Quit();

	return 0;
}