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
#include "struct.h"
#include "textureManager.h"
#include "interface.h"
#include "struct.h"

static const int x_offset = 100;
static const int y_offset = 50;
static const int sq_size = 40;




static const int milieu = BOARD_SIZE / 2;

static const SDL_Color white = {255, 255, 255, 255};
static const SDL_Color black = {0, 0, 0, 255};
static const SDL_Color darkGrey = {100, 100, 100, 255};
static const SDL_Color beige = {227,212,173, 255};


void SetColor (SDL_Renderer* renderer, SDL_Color color);


int InitializeRoboRicochet(GameStruct* g, char* id);

int RandomInt(int min, int max);
int RandomUnion(int min1, int max1, int min2, int max2);

int DeplaceGauche(SDL_Renderer* renderer, dimTexture* robots, dimTexture* jetons, Sommet* actuel,
				   Square board[BOARD_SIZE][BOARD_SIZE], Zipper* z);
int DeplaceDroite(SDL_Renderer* renderer, dimTexture* robots, dimTexture* jetons, Sommet* actuel,
				   Square board[BOARD_SIZE][BOARD_SIZE], Zipper* z);


int SetupEmptyBoard(SDL_Renderer* renderer);

int SetupNormalJetonsAndWalls(SDL_Renderer* renderer, dimTexture* jetons, Point positionJetons[16], Square board[BOARD_SIZE][BOARD_SIZE],
			bool horizontalWalls[BOARD_SIZE][BOARD_SIZE + 1], bool verticalWalls[BOARD_SIZE][BOARD_SIZE + 1]);
int SetupNormalRobots(SDL_Renderer* renderer, dimTexture* robots, Sommet* actuel, Square board[BOARD_SIZE][BOARD_SIZE]);


int SetupRandomJetonsAndWalls(SDL_Renderer* renderer, dimTexture* jetons, Point positionsJetons[16], Square board[BOARD_SIZE][BOARD_SIZE], 
	bool horizontalWalls[BOARD_SIZE][BOARD_SIZE + 1], bool verticalWalls[BOARD_SIZE][BOARD_SIZE + 1]);
int SetupRandomRobots(SDL_Renderer* renderer, dimTexture* robots, Square board[BOARD_SIZE][BOARD_SIZE], Sommet* actuel);

#endif //BOARDMANAGER

