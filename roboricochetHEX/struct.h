#ifndef STRUCT
#define STRUCT

#include "../include/SDL.h"
#include "../include/SDL_ttf.h"
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <time.h>
#include <string.h>

#define N_Robots 4

#define HEXSIZE 13
#define HEXSEMIHEIGHT 22

#define BOARD_RADIUS 9
#define BOARD_SIZE 19 //BOARD_SIZE = 2*BOARD_RADIUS + 1


/**************************TEXTURES***************************/

typedef struct Texture_Atlas {
	SDL_Texture* texture;
	SDL_Rect dimensions;
	int layer;

	int sub_w; 
	int sub_h;
	//For atlases ONLY --> they HAVE to be equal to 0 if the texture is not an atlas

} dimTexture;

/****************************BOARD****************************/

// Modification de Point :
typedef struct c {
	int q;
	int r;
} Point;

typedef enum element {
	NONE = -1,
	STAR,
	PLANET,
	TRIANGLE, 
	MOON
} Element;

typedef enum couleur {
	GREY = -1,
	BLUE,
	RED, 
	GREEN,
	YELLOW 
} Couleur;

typedef struct HexBox {
	bool startingBlock;
	SDL_Color startColor;

	bool finishHex;
	Element finishElement;
	Couleur finishColor;
} Hex;

/******************************GRAPHE*************************/

typedef struct Sommet {
	Point positions[N_Robots];
	int dist;
} Sommet;

typedef struct SommetList {
	Sommet elem;
	struct SommetList* suivant;
} SList;

typedef struct SommetMinStack {
	int capacite;
	int remplissage;
	Sommet* tableau;
} SMinStack;

typedef struct CheminList {
	Point positions[N_Robots];
	struct CheminList* suivant;
} Chemin;

typedef struct zipper {
	Chemin* gauche;
	Chemin* droite;
	int distance_totale;
} Zipper;

/****************************HACHAGEDIST*************************/

typedef struct HachageDist {
	SList** table;
	long unsigned w;
	long unsigned size;
} HashDist;

/****************************HACHAGE*************************/

typedef struct couple {
	Sommet cle;
	Sommet valeur;
} Couple;


typedef struct liste {
	Couple c;
	struct liste* suivant;
} CList;

typedef struct Hashtable {
	CList** table;
	long unsigned w;
	long unsigned size;
} HashTbl;

/****************INTERFACE****************/

typedef struct Bouton {
	SDL_Rect shape;
	SDL_Color outline_color;
	SDL_Color color;
	SDL_Texture* texte;
} Button;

/***************GAMESTRUCT****************/

typedef struct GS {
	SDL_Window* window;
	SDL_Renderer* renderer;
	dimTexture* jetons;
	dimTexture* robots;

	Zipper z;
	Sommet actuel;
	Hex board[BOARD_SIZE][BOARD_SIZE];
	bool VerticalWalls[BOARD_SIZE+1][BOARD_SIZE+1];
	bool DiagupWalls[BOARD_SIZE+1][BOARD_SIZE+1];
	bool DiagDownWalls[BOARD_SIZE+1][BOARD_SIZE+1];
	Point positionJetons[17];
	Button startButton;
} GameStruct;


#endif //STRUCT
