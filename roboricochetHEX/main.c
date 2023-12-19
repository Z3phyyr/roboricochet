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
#include "errorManager.h"
#include "boardManager.h"
#include "textureManager.h"

/*********************TODO****************************/
/*
	Il reste un peu à tout faire...
	Principale priorité : affichage !
	-> Réflechir à une carte bien fichue
	
	Puis adaptation de DIJKSTRA
*/
/*****************************************************/


#define FPSMILLISECONDLIMIT 33

const int window_w = 1200;
const int window_h = 800;

const long unsigned mersenne = 131071;


int main(int argc, char** argv) {
	GameStruct g;

	srand(time(NULL));
	
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

	/********************************************************************/
	bool is_running = true;
	
	g.jetons = CreateDimTextureFromImage(g.renderer, "C:/Program_Files/OCaml64/home/Admin/C/SDL/roboricochetSQUARE/images/jetons.bmp", 1, 100, 100);
	g.robots = CreateDimTextureFromImage(g.renderer, "C:/Program_Files/OCaml64/home/Admin/C/SDL/roboricochetSQUARE/images/robots.bmp", 1, 100, 100);


	/********************************************************************/

	throwwithCondition(InitializeRoboRicochet(&g, "") != 0, "Initialize RoboRicochet triggered an exception", &g);

	while(is_running) {
		SDL_Event e;
		while(SDL_PollEvent(&e)) {
			switch(e.type) {
				case SDL_QUIT: is_running = false;
					break;

				case SDL_KEYDOWN:
					switch (e.key.keysym.sym) {
						case SDLK_ESCAPE:
							is_running = false;
							break;
						case SDLK_r: 
							throwwithCondition(InitializeRoboRicochet(&g, "r") != 0, 
									"Initialize RoboRicochet triggered an exception", &g);
							break;
						case SDLK_d: 
							throwwithCondition(InitializeRoboRicochet(&g, "d") != 0, 
									"Initialize RoboRicochet triggered an exception", &g);
							break;
						case SDLK_p: 
							{
								for (int q=0; q<=BOARD_SIZE; q++) {
									for(int r=0; r<=BOARD_SIZE; r++) {
										printf("| %d |", g.DiagupWalls[q][r]);
									}
									printf("\n");
								}
								printf("\n\n");
							}
						default: break;
					}
					break;
				
				
				default: break;
			}
		}
		SDL_RenderPresent(g.renderer);
	}

	DestroyEverything(&g);
	TTF_Quit();
	SDL_Quit();

	return 0;
}