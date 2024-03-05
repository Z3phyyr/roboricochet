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
#include "astar.h"
#include "comparaison.h"

/*********************TODO****************************/
/*
	- Réfléchir à une heuristique sur les autres robots (i.e. limiter leurs déplacements)
	
	- Varier le nombre de robots et voir si il existe des chemins plus courts 
		(Heuristique ABSOLUMENT Necessaire dans ce cas)
	- Varier le nombre de murs
*/
/*****************************************************/


#define FPSMILLISECONDLIMIT 33

const int window_w = 1400;
const int window_h = 800;

const SDL_Color outline_bouton = {155, 155, 155, 255};
const SDL_Color couleur_bouton = {50, 50, 50, 255};
const SDL_Rect info_text_rect = {750, 100, 450, 200};



void HandleMouseClicks(GameStruct* g, SDL_Event* e) {
	//Start button pressed
	if (CheckBoutonPresse(&g->startButton, e->motion.x, e->motion.y)) {
		FreeZipper(g->z);

		//Jeton selection
		int jeton;
		if (inBounds(g->choixJ, 0, 16)) {
			jeton = g->choixJ;
		} else {
			jeton = RandomInt(0, 16);
		}
		Element e = jeton / 4;
		Couleur c = jeton % 4;

		//Central token display
		SetColor(g->renderer, black);
		static const SDL_Rect centre = {x_offset + (milieu-1)*sq_size, y_offset + (milieu-1)*sq_size, 2*sq_size, 2*sq_size};
		throwwithCondition(SDL_RenderFillRect(g->renderer, &centre) || 
		    PrintSubTexture(g->renderer, g->jetons, x_offset + (BOARD_SIZE/2.0 - 0.5)*sq_size, 
		    y_offset + (BOARD_SIZE/2.0 - 0.5)*sq_size, c, e, sq_size) < 0, 
		    "Central Token display triggered an exception", g);
		SetColor(g->renderer, white);
		throwwithCondition(SDL_RenderFillRect(g->renderer, &info_text_rect) != 0, "Erasing the info text triggered an exception", g);
		
		SDL_RenderPresent(g->renderer);

		//Selection of the algorithm 
		if (!inBounds(g->choixH, 0, NB_HEURISTIQUES-1)) {
			g->choixH = RandomInt(0, NB_HEURISTIQUES-1);
		}
		clock_t t1 = clock();
		switch (g->choixH) {
			case 0:
				g->z = Dijkstra(g->actuel, g->positionsJetons[jeton], c, g->horizontalWalls, g->verticalWalls);
				break;

			case 1:
				g->z = a_star(g->actuel, g->positionsJetons[jeton], c, h_euclidienne, g->horizontalWalls, g->verticalWalls);
				break;

			case 2:
				g->z = a_star(g->actuel, g->positionsJetons[jeton], c, h_manhattan, g->horizontalWalls, g->verticalWalls);
				break;	
			case 3:
				g->z = a_star(g->actuel, g->positionsJetons[jeton], c, h_3, g->horizontalWalls, g->verticalWalls);		
		}
		clock_t t2 = clock();
		printf("Temps d'éxécution = %f\n", (double)(t2 - t1) / CLOCKS_PER_SEC);
		
		AffichageTexteInformatif(g->renderer, g->z.distance_totale);
			
		g->actuel.dist = 0;
	}

	//Heuristique Button Pressed
	for(int k=0; k<NB_HEURISTIQUES; k++) {
		if (CheckBoutonPresse(&g->boutonsHeuristiques[k], e->motion.x, e->motion.y)) {
			g->choixH = k;
			AffichageHeuristiqueChoisie(g->renderer, g->choixH);
		}
	}

	//Jeton Button Pressed
	for(int k=0; k<NB_JETONS+1; k++) {
		if (CheckBoutonPresse(&g->boutonsJetons[k], e->motion.x, e->motion.y)) {
			g->choixJ = k;
			AffichageJetonChoisi(g->renderer, g->choixJ);
		}
	}
}



int main (int argc, char** argv) {

	GameStruct g;

	if (SDL_Init(SDL_INIT_VIDEO) != 0 || TTF_Init() != 0) {
		SDL_ExitWithError("Error during Initialisation");
	}

	g.window = SDL_CreateWindow("RoboRicochet Square", 50, 25, window_w, window_h, 0);
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
	char* init_str = "r";

	//Génération aléatoire du plateau si pas la bonne dimension
	if (BOARD_SIZE != 16) {
		strcpy(init_str, "y");
	}

	g.jetons = CreateDimTextureFromImage(g.renderer, "C:/Program_Files/OCaml64/home/Admin/C/SDL/roboricochetSQUARE/images/jetons.bmp", 1, 100, 100);
	g.robots = CreateDimTextureFromImage(g.renderer, "C:/Program_Files/OCaml64/home/Admin/C/SDL/roboricochetSQUARE/images/robots.bmp", 1, 100, 100);
	g.choixH = -1;
	g.choixJ = -1;

	g.actuel = (Sommet) {{{1, 2}, {0, 0}, {7, 4}, {12, 5}}, 0};
	g.z = (Zipper) {NULL, NULL, 0};

	g.startButton = (Button) {(SDL_Rect) {900, 450, -1, -1}, outline_bouton, couleur_bouton, NULL};
	throwwithCondition(SetButtonTextTexture(g.renderer, &g.startButton, "Start", 30, consola, blueButton) != 0, "GetButtonTextTexture triggered an exception", &g);
	
	g.boutonsHeuristiques = InitBoutonsHeuristiques(g.renderer);
	g.boutonsJetons = InitBoutonsJetons(g.renderer, g.jetons);
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
					HandleMouseClicks(&g, &e);
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
							throwwithCondition(DeplaceGauche(g.renderer, g.robots, g.jetons, &g.actuel, g.board, &g.z) != 0, "DeplaceGauche triggered an exception", &g);
							AffichagePosition(g.renderer, g.actuel.dist, g.z.distance_totale);
							break;
						case SDLK_RIGHT: 
							throwwithCondition(DeplaceDroite(g.renderer, g.robots, g.jetons, &g.actuel, g.board, &g.z) != 0, "DeplaceDroite triggered an exception", &g);
							AffichagePosition(g.renderer, g.actuel.dist, g.z.distance_totale);
							break;

						case SDLK_t:
							recuperer(&g, "output.txt");
							break;

						case SDLK_f:
							FreeZipper(g.z);
							
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
	TTF_Quit();
	SDL_Quit();
	
	return 0;
}