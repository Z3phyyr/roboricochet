#include "../include/SDL.h"
#include "../include/SDL_ttf.h"

#include <assert.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

/*************************BOARD MANAGER**************************/

typedef struct P {
	int q;
	int r;
} Point;

typedef enum Couleur {
	GREY = -1,
	BLUE,
	RED,
	YELLOW, 
	GREEN
} Couleur;

typedef enum element {
	NONE = -1,
	STAR,
	PLANET,
	TRIANGLE, 
	MOON
} Element;

typedef struct HexBox {
	

	bool startingBlock;
	SDL_Color startColor;

	bool finishHex;
	Element e;
	Couleur c;
} Hex;