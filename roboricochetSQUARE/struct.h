#ifndef STRUCT
#define STRUCT

#include "../include/SDL.h"
#include "../include/SDL_ttf.h"
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include "textureManager.h"

#define N_Robots 4
#define NB_HEURISTIQUES 4
#define NB_JETONS 17

//IMPERATIVEMENT DEFINIR UN ENTIER PAIR; MINIMUM 8 !!!!!!!
#define BOARD_SIZE 16

static const int milieu = BOARD_SIZE / 2;
static const SDL_Color blueButton = {100, 100, 255, 255};
static const SDL_Color white = {255, 255, 255, 255};
static const SDL_Color black = {0, 0, 0, 255};
static const SDL_Color darkGrey = {100, 100, 100, 255};
static const SDL_Color beige = {227,212,173, 255};
static const SDL_Color blueviolet = {138, 43, 226, 255};
static const SDL_Color magenta = {255, 0, 255, 255};
static const char consola[] = "C:/Program_Files/OCaml64/home/Admin/C/SDL/roboricochetSQUARE/fonts/consola.ttf";
static const char FixedSys[] = "C:/Program_Files/OCaml64/home/Admin/C/SDL/roboricochetSQUARE/fonts/FixedSys.ttf";
	

/****************************BOARD****************************/

typedef struct c {
	int i;
	int j;
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
	YELLOW,
	GREEN 
} Couleur;

typedef struct sq {
	bool startingBlock;
	SDL_Color startColor;

	bool finishSquare;
	Element finishElement;
	Couleur finishColor;
} Square;

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
	int w;
	int size;
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
	int w;
	int size;
} HashTbl;

/******************************INTERFACE*************************/

typedef struct Bouton {
	SDL_Rect shape;
	SDL_Color outline_color;
	SDL_Color color;
	SDL_Texture* texture;
} Button;

/*****************************GAMESTRUCT*************************/


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
	Square board[BOARD_SIZE][BOARD_SIZE];
	bool horizontalWalls[BOARD_SIZE][BOARD_SIZE + 1];
	bool verticalWalls[BOARD_SIZE][BOARD_SIZE + 1];
} GameStruct;

#endif //STRUCT
