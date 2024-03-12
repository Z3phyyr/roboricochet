#ifndef BOARDMANAGER
#define BOARDMANAGER





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

static const SDL_Color sb_red = {160, 0, 5, 150};
static const SDL_Color sb_blue = {23, 78, 134, 150};
static const SDL_Color sb_green = {0, 127, 12, 150};
static const SDL_Color sb_yellow = {240, 244, 0, 150};




void SetColor (SDL_Renderer* renderer, SDL_Color color);
void RandomPositionPasCentre(int* i, int* j);

int InitializeRoboRicochet(GameStruct* g, char* id);

int RandomInt(int min, int max);
int RandomUnion(int min1, int max1, int min2, int max2);

bool Connexe (Square board[BOARD_SIZE][BOARD_SIZE], Point s, Point t,
			  bool horizontalWalls[BOARD_SIZE][BOARD_SIZE + 1], 
			  bool verticalWalls[BOARD_SIZE][BOARD_SIZE + 1]);

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

