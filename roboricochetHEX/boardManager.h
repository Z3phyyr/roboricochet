#ifndef BOARDMANAGER
#define BOARDMANAGER

#include "../include/SDL.h"
#include "../include/SDL_ttf.h"

#include <assert.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdlib.h>
#include "textureManager.h"
#include "struct.h"

#define HEXSIZE 13

#define BOARD_RADIUS 9
#define BOARD_SIZE 19 //BOARD_SIZE = 2*BOARD_RADIUS + 1

#define N_Robots 4

typedef enum Walltype {
	VERTICAL,
	DIAGUP,
	DIAGDOWN
} WallType;


typedef struct GS {
	SDL_Window* window;
	SDL_Renderer* renderer;
	dimTexture* jetons;
	dimTexture* robots;

	Hex board[BOARD_SIZE][BOARD_SIZE];
	bool VerticalWalls[BOARD_SIZE+1][BOARD_SIZE+1];
	bool DiagupWalls[BOARD_SIZE+1][BOARD_SIZE+1];
	bool DiagDownWalls[BOARD_SIZE+1][BOARD_SIZE+1];
	Point positionJetons[16];
} GameStruct;

void GetTopCoordinates(int q, int r, int* x, int* y);

int DrawVerticalWall(SDL_Renderer* renderer, int q_left, int r);
int DrawDiagUpWall(SDL_Renderer* renderer, int q, int r_bot);
int DrawDiagDownWall(SDL_Renderer* renderer, int q_bot, int r_bot);

int SetupEmptyBoard(SDL_Renderer *renderer);
int SetupNormalJetonsAndWalls(SDL_Renderer *renderer, dimTexture *jetons, Hex board[BOARD_SIZE][BOARD_SIZE], Point positionJetons[16],
		bool VerticalWalls[BOARD_SIZE+1][BOARD_SIZE+1],
		bool DiagupWalls[BOARD_SIZE+1][BOARD_SIZE+1],
		bool DiagDownWalls[BOARD_SIZE+1][BOARD_SIZE+1]);

int SetupRandomJetonsAndWalls(SDL_Renderer *renderer, dimTexture *jetons, 
		Hex board[BOARD_SIZE][BOARD_SIZE], Point positionsJetons[16],
		bool VerticalWalls[BOARD_SIZE+1][BOARD_SIZE+1],
		bool DiagupWalls[BOARD_SIZE+1][BOARD_SIZE+1],
		bool DiagDownWalls[BOARD_SIZE+1][BOARD_SIZE+1]);
int SetupRandomRobots(SDL_Renderer* renderer, dimTexture* robots, Hex board[BOARD_SIZE][BOARD_SIZE]);


int InitializeRoboRicochet(GameStruct* g, char* id);


#endif //BOARDMANAGER