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
#include "interface.h"
#include "graphe.h"
#include "errorManager.h"
#include "boardManager.h"
#include "textureManager.h"
#include "comparaison.h"

/*********************TODO****************************/
/*
	Implémenter la fin du défilement de l'affichage
*/
/*****************************************************/



#define FPSMILLISECONDLIMIT 33

const int window_w = 1400;
const int window_h = 800;

const SDL_Color outline_bouton = {155, 155, 155, 255};
const SDL_Color couleur_bouton = {50, 50, 50, 255};







void HandleMouseClicks(GameStruct* g, SDL_Event* e) {
	if (CheckBoutonPresse(&g->startButton, e->motion.x, e->motion.y)) {
		FreeZipper(&g->z);

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
		throwwithCondition(FillCenterHexagon(g->renderer) != 0 
							|| DrawJeton(g->renderer, g->jetons, jeton, (Point){BOARD_RADIUS, BOARD_RADIUS}) != 0, 
							"Central token display triggered an exception", g);
		SDL_RenderPresent(g->renderer);

		//Selection of Algorithm
		if (!inBounds(g->choixH, 0, NB_HEURISTIQUES-1)) {
			g->choixH = RandomInt(0, NB_HEURISTIQUES-1);
		}
		clock_t t1 = clock();
		switch (g->choixH) {
					case 0:
						g->z = Dijkstra(g->actuel, g->positionJetons[jeton], c, g->VerticalWalls, g->DiagupWalls, g->DiagDownWalls);
						break;
					case 1:
						g->z = a_star(g->actuel, g->positionJetons[jeton], c, h_euclidienne, g->VerticalWalls, g->DiagupWalls, g->DiagDownWalls);
						break;
					case 2:
						g->z = a_star(g->actuel, g->positionJetons[jeton], c, h_manhattan, g->VerticalWalls, g->DiagupWalls, g->DiagDownWalls);
						break;	
					case 3:
						g->z = a_star(g->actuel, g->positionJetons[jeton], c, h_rapide, g->VerticalWalls, g->DiagupWalls, g->DiagDownWalls);
						break;
				}
		clock_t t2 = clock();
		printf("Temps d'exécution : %f\n", ((double)(t2-t1))/CLOCKS_PER_SEC);
		AffichageTexteInformatif(g->renderer, g->z.distance_totale);
		g->actuel.dist = 0;
	}
	for(int k=0; k<NB_HEURISTIQUES; k++) {
		if (CheckBoutonPresse(&g->boutonsHeuristiques[k], e->motion.x, e->motion.y)) {
			g->choixH = k;
			AffichageHeuristiqueChoisie(g->renderer, g->choixH);
		}
	}
	for (int k=0; k<NB_JETONS; k++) {
		if (CheckBoutonPresse(&g->boutonsJetons[k], e->motion.x, e->motion.y)) {
			g->choixJ = k;
			AffichageJetonChoisi(g->renderer, g->choixJ);
		}
	}
}

void HandleKeyPresses(GameStruct* g, SDL_Event* e, bool* is_running) {
	switch (e->key.keysym.sym) {
		case SDLK_ESCAPE:
			*is_running = false;
			break;
		case SDLK_r: 
			throwwithCondition(InitializeRoboRicochet(g, "r") != 0, 
				"Initialize RoboRicochet triggered an exception", g);
			break;
		case SDLK_w: 
			throwwithCondition(InitializeRoboRicochet(g, "w") != 0, 
				"Initialize RoboRicochet triggered an exception", g);
			break;
		case SDLK_d:
			throwwithCondition(InitializeRoboRicochet(g, "d") != 0, 
				"Initialize RoboRicochet triggered an exception", g);
			break;
				
		case SDLK_RIGHT:
			throwwithCondition(DeplaceDroite(g->renderer, g->robots, g->jetons, &g->actuel, g->board, &g->z), "DeplaceDroite triggered an exception", g);
			AffichagePosition(g->renderer, g->actuel.dist, g->z.distance_totale);							
			break;
						
		case SDLK_LEFT:
			throwwithCondition(DeplaceGauche(g->renderer, g->robots, g->jetons, &g->actuel, g->board, &g->z), "DeplaceGauche triggered an exception", g);
			AffichagePosition(g->renderer, g->actuel.dist, g->z.distance_totale);
			break;

		case SDLK_t:
			recuperer(g, "outputHEX.txt");
			break;

		case SDLK_k: { 
			printf("Actuel :\n");
			int q, r, s;
			for (int k=0; k<N_Robots; k++) {
				q = g->actuel.positions[k].q;
				r = g->actuel.positions[k].r;
				s = q + r - 9;
				printf("(%d, %d, %d) ", q, r, s);
			}
			printf("\n\n");
			SList* desc = DescendantsDirects(g->actuel, g->VerticalWalls, g->DiagupWalls, g->DiagDownWalls);
			AfficherSList(desc);
			FreeSList(desc);
			}
			break;
						
			default:break;
	}
}

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
	g.choixH = -1;
	g.choixJ = -1;

	g.startButton = (Button){(SDL_Rect){1000, 150, -1, -1}, 
					outline_bouton, couleur_bouton, 
					NULL};
	g.z = (Zipper) {NULL, NULL, 0};
	
	throwwithCondition(SetButtonTextTexture(g.renderer, &g.startButton, "Start the algorithm", 25, consola, black) != 0, "ButtonInitialization triggered an exception", &g);
	g.boutonsJetons = InitBoutonsJetons(g.renderer, g.jetons);
	g.boutonsHeuristiques = InitBoutonsHeuristiques(g.renderer);
	throwwithCondition(g.boutonsJetons == NULL || g.boutonsHeuristiques == NULL, "Buttons initialization triggered an exception", &g); 
	
	/********************************************************************/

	throwwithCondition(InitializeRoboRicochet(&g, "") != 0, "Initialize RoboRicochet triggered an exception", &g);
	
	
	while(is_running) {
		SDL_Event e;
		while(SDL_PollEvent(&e)) {
			switch(e.type) {
				case SDL_QUIT: is_running = false;
					break;

				case SDL_MOUSEBUTTONDOWN:
					HandleMouseClicks(&g, &e);
					break;

				case SDL_KEYDOWN:
					HandleKeyPresses(&g, &e, &is_running);
					break;
				
				
				default: break;
			}
		}
		SDL_Delay(FPSMILLISECONDLIMIT);
		SDL_RenderPresent(g.renderer);
	}

	DestroyEverything(&g);
	TTF_Quit();
	SDL_Quit();

	return 0;
}