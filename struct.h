#ifndef STRUCT
#define STRUCT

#include "../include/SDL.h"
#include "../include/SDL_ttf.h"
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>

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
} HashTbl;

#endif //STRUCT
