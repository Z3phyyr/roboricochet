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
#include "interface.h"

/*********************TODO****************************/
/*
	- Affichage : "Chemin trouvé en n coups:  
				   Position i/n" puis défiler
				  Ptet utilise TTF_RenderTextWrapped() (ou quelque chose comme ça)
				  Réfléchir à comment afficher des nombres
	- Réfléchir à une heuristique sur les autres robots (i.e. limiter leurs déplacements)
	
	- Varier le nombre de robots et voir si il existe des chemins plus courts 
		(Heuristique ABSOLUMENT Necessaire dans ce cas)
	- Varier le nombre de murs
*/
/*****************************************************/


#define FPSMILLISECONDLIMIT 33

const int window_w = 1200;
const int window_h = 800;

const SDL_Color outline_bouton = {155, 155, 155, 255};
const SDL_Color couleur_bouton = {50, 50, 50, 255};



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

	g.jetons = CreateDimTextureFromImage(g.renderer, "C:/Program_Files/OCaml64/home/Admin/C/SDL/roboricochetSQUARE/images/jetons.bmp", 1, 100, 100);
	g.robots = CreateDimTextureFromImage(g.renderer, "C:/Program_Files/OCaml64/home/Admin/C/SDL/roboricochetSQUARE/images/robots.bmp", 1, 100, 100);


	g.actuel = (Sommet) {{{1, 2}, {0, 0}, {7, 4}, {12, 5}}, 0};
	Zipper z = {NULL, NULL};

	g.startButton = (Button) {(SDL_Rect) {900, 450, 50, 20}, outline_bouton, couleur_bouton};

	/******************************************/
	throwwithCondition(InitializeRoboRicochet(&g, "yousk2") < 0, "Error during Game Initialisation", &g);
	
	
	while (is_running) {
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
				case SDL_QUIT:
					is_running = false;
					break;

				case SDL_MOUSEBUTTONDOWN:
					if (CheckBoutonPresse(&g.startButton, e.motion.x, e.motion.y)) {
						if (cheminpresent) {
							FreeZipper(z);

						}
						int Choix = RandomInt(0, 15);
						Element e = Choix / 4;
						Couleur c = Choix % 4;
						SetColor(g.renderer, black);
						static const SDL_Rect centre = {x_offset + (milieu-1)*sq_size, y_offset + (milieu-1)*sq_size, 2*sq_size, 2*sq_size};
						throwwithCondition(SDL_RenderFillRect(g.renderer, &centre) || 
										   PrintSubTexture(g.renderer, g.jetons, x_offset + (BOARD_SIZE/2.0 - 0.5)*sq_size, 
										   y_offset + (BOARD_SIZE/2.0 - 0.5)*sq_size, c, e, sq_size) < 0, 
										   "Central Token display triggered an exception", &g);
						SDL_RenderPresent(g.renderer);

						printf("Lancement de DIJKSTRA\n");
						z = Dijkstra(g.actuel, g.positionsJetons[Choix], c, g.horizontalWalls, g.verticalWalls);
						AffichageTexteInformatif(g.renderer, z.distance_totale);
						cheminpresent = true;	
						g.actuel.dist = 0;
					}
					break;
				
				case SDL_KEYDOWN: 
					switch (e.key.keysym.sym) {
						case SDLK_ESCAPE:
							is_running = false;
							break;
						case SDLK_d:
							throwwithCondition(InitializeRoboRicochet(&g, "z") < 0, "Error during initialisation", &g);
							g.actuel.dist = 0;
							break;
						case SDLK_r:
							throwwithCondition(InitializeRoboRicochet(&g, "r") < 0, "Error during initialisation", &g);
							g.actuel.dist = 0;
							break;
						

						case SDLK_LEFT:
							if (cheminpresent) {
								throwwithCondition(DeplaceGauche(g.renderer, g.robots, g.jetons, &g.actuel, g.board, &z) != 0, "DeplaceGauche triggered an exception", &g);
								AffichagePosition(g.renderer, g.actuel.dist, z.distance_totale);
							}
							break;
						case SDLK_RIGHT: 
							if (cheminpresent) {
								throwwithCondition(DeplaceDroite(g.renderer, g.robots, g.jetons, &g.actuel, g.board, &z) != 0, "DeplaceDroite triggered an exception", &g);
								AffichagePosition(g.renderer, g.actuel.dist, z.distance_totale);
							}
							
							break;


						case SDLK_f:
							FreeZipper(z);
							cheminpresent = false;
							
							SDL_SetRenderDrawColor(g.renderer, 255, 255, 255, 255);
							SDL_Rect rect = {900, 0, 300, window_h};
							SDL_RenderDrawRect(g.renderer,  &rect);
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