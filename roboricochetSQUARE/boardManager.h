#ifndef BOARDMANAGER
#define BOARDMANAGER

//IMPERATIVEMENT DEFINIR UN ENTIER PAIR; MINIMUM 8 !!!!!!!
#define BOARD_SIZE 16



#include "../include/SDL.h"
#include "../include/SDL_ttf.h"

#include <assert.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "textureManager.h"
#include "struct.h"
//#include "graphe.h"




typedef struct GS {
	SDL_Window* window;
	SDL_Renderer* renderer;
	dimTexture* jetons;
	dimTexture* robots;

	Sommet actuel;

	Square board[BOARD_SIZE][BOARD_SIZE];
	bool horizontalWalls[BOARD_SIZE][BOARD_SIZE + 1];
	bool verticalWalls[BOARD_SIZE][BOARD_SIZE + 1];

	int sq_size;
} GameStruct;


int InitializeRoboRicochet(GameStruct* g, char* id);

int RandomInt(int min, int max);
int RandomUnion(int min1, int max1, int min2, int max2);


int DeplaceGauche(SDL_Renderer* renderer, dimTexture* robots, Sommet* actuel,
				   Square board[BOARD_SIZE][BOARD_SIZE], Zipper* z);
int DeplaceDroite(SDL_Renderer* renderer, dimTexture* robots, Sommet* actuel,
				   Square board[BOARD_SIZE][BOARD_SIZE], Zipper* z);


int SetupEmptyBoard(SDL_Renderer* renderer, Square board[BOARD_SIZE][BOARD_SIZE]);

int SetupNormalJetonsAndWalls(SDL_Renderer* renderer, dimTexture* jetons, Square board[BOARD_SIZE][BOARD_SIZE],
			bool horizontalWalls[BOARD_SIZE][BOARD_SIZE + 1], bool verticalWalls[BOARD_SIZE][BOARD_SIZE + 1]);
int SetupNormalRobots(SDL_Renderer* renderer, dimTexture* robots, Square board[BOARD_SIZE][BOARD_SIZE]);


int SetupRandomJetonsAndWalls(SDL_Renderer* renderer, dimTexture* jetons, Square board[BOARD_SIZE][BOARD_SIZE], 
	bool horizontalWalls[BOARD_SIZE][BOARD_SIZE + 1], bool verticalWalls[BOARD_SIZE][BOARD_SIZE + 1]);
int SetupRandomRobots(SDL_Renderer* renderer, dimTexture* robots, Square board[BOARD_SIZE][BOARD_SIZE]);

#endif //BOARDMANAGER

