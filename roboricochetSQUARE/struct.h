#ifndef STRUCT
#define STRUCT

#include "../include/SDL.h"
#include "../include/SDL_ttf.h"
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include "textureManager.h"
#include "boardManager.h"

#define N_Robots 4


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

/******************************GRAPHE/Minstack*************************/

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

// nouveau
typedef struct PrioMinStack {
    int capacite;
    int remplissage;
    Sommet* tableau;
    double* priorite;
} PMinStack;

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
	//const char* texte;
} Button;

/*****************************GAMESTRUCT*************************/


typedef struct GS {
	SDL_Window* window;
	SDL_Renderer* renderer;
	dimTexture* jetons;
	dimTexture* robots;

	Sommet actuel;
	Point positionsJetons[16];
	Button startButton;

	Square board[BOARD_SIZE][BOARD_SIZE];
	bool horizontalWalls[BOARD_SIZE][BOARD_SIZE + 1];
	bool verticalWalls[BOARD_SIZE][BOARD_SIZE + 1];
	
	Zipper z;
} GameStruct;

#endif //STRUCT
