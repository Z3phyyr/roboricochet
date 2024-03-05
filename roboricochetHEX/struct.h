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
#define NB_JETONS 17
#define NB_HEURISTIQUES 4

static const SDL_Color blueButton = {100, 100, 255, 255};
static const SDL_Color white = {255, 255, 255, 255};
static const SDL_Color black = {0, 0, 0, 255};
static const SDL_Color darkGrey = {100, 100, 100, 255};
static const SDL_Color beige = {227,212,173, 255};
static const SDL_Color blueviolet = {138, 43, 226, 255};
static const SDL_Color magenta = {255, 0, 255, 255};
static const char consola[] = "C:/Program_Files/OCaml64/home/Admin/C/SDL/roboricochetSQUARE/fonts/consola.ttf";
static const char FixedSys[] = "C:/Program_Files/OCaml64/home/Admin/C/SDL/roboricochetSQUARE/fonts/FixedSys.ttf";


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
	SDL_Texture* texture;
} Button;

/***************GAMESTRUCT****************/

typedef struct GS {
	SDL_Window* window;
	SDL_Renderer* renderer;
	dimTexture* jetons;
	dimTexture* robots;

	Sommet actuel;
	Point positionsJetons[NB_JETONS];
	Button startButton;

	Button* boutonsHeuristiques;
	Button* boutonsJetons;
	int choixH;
	int choixJ;


	Zipper z;	
	Point positionJetons[NB_JETONS];
	Hex board[BOARD_SIZE][BOARD_SIZE];
	bool VerticalWalls[BOARD_SIZE+1][BOARD_SIZE+1];
	bool DiagupWalls[BOARD_SIZE+1][BOARD_SIZE+1];
	bool DiagDownWalls[BOARD_SIZE+1][BOARD_SIZE+1];
} GameStruct;


#endif //STRUCT
