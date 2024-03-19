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
#include "interface.h"
#include "graphe.h"



typedef enum Walltype {
	VERTICAL,
	DIAGUP,
	DIAGDOWN
} WallType;


void SetColor(SDL_Renderer *renderer, SDL_Color color);
int RandomInt(int min, int max);
Point RandomPoint(const int r_min[BOARD_SIZE], const int r_max[BOARD_SIZE]);
void GetTopCoordinates(int q, int r, int* x, int* y);
void GetPrintingCoordinates(int q, int r, int* x, int* y);

int FillCenterHexagon(SDL_Renderer* renderer); 
int DrawVerticalWall(SDL_Renderer* renderer, int q_left, int r);
int DrawDiagUpWall(SDL_Renderer* renderer, int q, int r_bot);
int DrawDiagDownWall(SDL_Renderer* renderer, int q_bot, int r_bot);
int DrawJeton(SDL_Renderer* renderer, dimTexture* jetons, const int numero, Point position);

bool Connexe(Hex board[BOARD_SIZE][BOARD_SIZE], Point s, Point t, 
			 bool VerticalWalls[BOARD_SIZE+1][BOARD_SIZE+1],
			 bool DiagupWalls[BOARD_SIZE+1][BOARD_SIZE+1],
			 bool DiagDownWalls[BOARD_SIZE+1][BOARD_SIZE+1]); 

int DeplaceDroite(SDL_Renderer *renderer, dimTexture *robots, dimTexture *jetons, Sommet *actuel, Hex board[BOARD_SIZE][BOARD_SIZE], Zipper *z);
int DeplaceGauche(SDL_Renderer* renderer, dimTexture* robots, dimTexture* jetons, Sommet* actuel, Hex board[BOARD_SIZE][BOARD_SIZE], Zipper* z);

int SetupEmptyBoard(SDL_Renderer *renderer);
int SetupNormalJetonsAndWalls(SDL_Renderer *renderer, dimTexture *jetons, Hex board[BOARD_SIZE][BOARD_SIZE], Point positionJetons[16],
		bool VerticalWalls[BOARD_SIZE+1][BOARD_SIZE+1],
		bool DiagupWalls[BOARD_SIZE+1][BOARD_SIZE+1],
		bool DiagDownWalls[BOARD_SIZE+1][BOARD_SIZE+1]);
int SetupNormalRobots(SDL_Renderer* renderer, dimTexture* robots, Sommet* actuel, Hex board[BOARD_SIZE][BOARD_SIZE]);

int SetupRandomJetonsAndWalls(SDL_Renderer *renderer, dimTexture *jetons, 
		Hex board[BOARD_SIZE][BOARD_SIZE], Point positionsJetons[16],
		bool VerticalWalls[BOARD_SIZE+1][BOARD_SIZE+1],
		bool DiagupWalls[BOARD_SIZE+1][BOARD_SIZE+1],
		bool DiagDownWalls[BOARD_SIZE+1][BOARD_SIZE+1]);
int SetupRandomRobots(SDL_Renderer* renderer, dimTexture* robots, Sommet* actuel, Hex board[BOARD_SIZE][BOARD_SIZE]);


int InitializeRoboRicochet(GameStruct* g, char* id);


#endif //BOARDMANAGER