#include "boardManager.h"


const SDL_Color sb_red = {160, 0, 5, 150};
const SDL_Color sb_blue = {23, 78, 134, 150};
const SDL_Color sb_green = {0, 127, 12, 150};
const SDL_Color sb_yellow = {240, 244, 0, 150};



void SetColor (SDL_Renderer* renderer, SDL_Color color) {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

int RandomInt(int min, int max) {
	int mod = SDL_abs(max - min) + 1;
	return rand() % mod + min;
}

int RandomUnion (int min1, int max1, int min2, int max2) {
	int u = RandomInt(0, 1);

	if (u == 1) return RandomInt(min1, max1);

	return RandomInt(min2, max2);
}

void RandomPositionPasCentre(int* i, int* j) {
	*i = RandomInt(1, BOARD_SIZE - 2);
	if (*i == milieu - 1 || *i == milieu) {
		*j = RandomUnion(1, milieu - 2, milieu + 1, BOARD_SIZE - 2);
	} else {
		*j = RandomInt(1, BOARD_SIZE - 2);
	}
}

int PrintVerticalWall (SDL_Renderer* renderer, int k, int m) {
	SDL_Rect rect = {x_offset + m*sq_size - 2, y_offset + k*sq_size, 4, sq_size};
	return SDL_RenderFillRect(renderer, &rect);
}

int PrintHorizontalWall(SDL_Renderer* renderer, int k , int m) {
	SDL_Rect rect = {x_offset + k*sq_size, y_offset + m*sq_size - 2, sq_size, 4};
	return SDL_RenderFillRect(renderer, &rect);
}

int DrawStartingBlock(SDL_Renderer* renderer, int i, int j, SDL_Color color) {
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SetColor(renderer, color);
	int x = x_offset + i*sq_size;
	int y = y_offset + j*sq_size;
	const SDL_Rect rect = {x + 2, y + 2, sq_size - 4, sq_size - 4};
	if (SDL_RenderFillRect(renderer, &rect) < 0) return -1;
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
	return 0;
}


/***************************AFFICHAGE DES CHEMINS***********************/





int DeplaceGauche(SDL_Renderer* renderer, dimTexture* robots, dimTexture* jetons, Sommet* actuel,
				   Square board[BOARD_SIZE][BOARD_SIZE], Zipper* z) {
	
	if (z->gauche == NULL) {
		return 0;
	}

	//Copie de actuel en tête de droite
		Chemin* new = malloc(sizeof(Chemin));
		for (int k=0; k<N_Robots; k++) {
			new->positions[k] = actuel->positions[k];
		}
		new->suivant = z->droite;
		z->droite = new;
	

	for (int k=0; k<N_Robots; k++) {
		Point p = z->gauche->positions[k];
		if (actuel->positions[k].i != p.i || actuel->positions[k].j != p.j) {
			int x = x_offset + p.i*sq_size + 2;
			int y = y_offset + p.j*sq_size + 2;
			Square old_sq = board[actuel->positions[k].i][actuel->positions[k].j];
			int x_actuel = x_offset + actuel->positions[k].i*sq_size + 2;
			int y_actuel = y_offset + actuel->positions[k].j*sq_size + 2;
			
			//Affiche le robot sur la nouvelle case
			if (PrintSubTexture(renderer, robots, x, y, 
						k, 0, sq_size - 4) != 0) return -1;

			//Efface la case précédente
			SetColor(renderer, beige);
			SDL_Rect rect = {x_actuel, y_actuel, sq_size - 4, sq_size - 4};
			if (SDL_RenderFillRect(renderer, &rect) != 0) return -1;

			//Affiche si besoin le jeton ou la case de départ sur l'ancienne case
			if (old_sq.startingBlock) {
				if (DrawStartingBlock(renderer, actuel->positions[k].i, 
						actuel->positions[k].j, old_sq.startColor) != 0) {
					return -1;
				}
			}
			if (old_sq.finishSquare) {
				if(PrintSubTexture(renderer, jetons, x_actuel, y_actuel, 
								old_sq.finishColor, old_sq.finishElement, 
								sq_size - 4) != 0) {
					return -1;
				}
			}

			//Copie de la tête du chemin de gauche dans actuel
			(*actuel).positions[k] = p;
			actuel->dist--;
		}		
	}
	//Suppression de la tête du chemin de gauche 
		Chemin* suivantGauche = z->gauche->suivant;
		free(z->gauche);
		z->gauche = suivantGauche;

	return 0;	
}

int DeplaceDroite(SDL_Renderer* renderer, dimTexture* robots, dimTexture* jetons, Sommet* actuel,
				   Square board[BOARD_SIZE][BOARD_SIZE], Zipper* z) {
	
	if (z->droite == NULL) {
		return 0;
	}

	//Copie de actuel en tête de gauche
		Chemin* new = malloc(sizeof(Chemin));
		for (int k=0; k<N_Robots; k++) {
			new->positions[k] = actuel->positions[k];
		}
		new->suivant = z->gauche;
		z->gauche = new;
	

	for (int k=0; k<N_Robots; k++) {
		Point p = z->droite->positions[k];
		if (actuel->positions[k].i != p.i || actuel->positions[k].j != p.j) {
			int x = x_offset + p.i*sq_size + 2;
			int y = y_offset + p.j*sq_size + 2;
			Square old_sq = board[actuel->positions[k].i][actuel->positions[k].j];
			int x_actuel = x_offset + actuel->positions[k].i*sq_size + 2;
			int y_actuel = y_offset + actuel->positions[k].j*sq_size + 2;
			
			//Affiche le robot sur la nouvelle case
			if (PrintSubTexture(renderer, robots, x, y, 
						k, 0, sq_size - 4) != 0) return -1;

			//Efface la case précédente
			SetColor(renderer, beige);
			SDL_Rect rect = {x_actuel, y_actuel, sq_size - 4, sq_size - 4};
			if (SDL_RenderFillRect(renderer, &rect) != 0) return -1;

			//Affiche si besoin le jeton ou la case de départ sur l'ancienne case
			if (old_sq.startingBlock) {
				if (DrawStartingBlock(renderer, actuel->positions[k].i, 
						actuel->positions[k].j, old_sq.startColor) != 0) {
					return -1;
				}
			}
			if (old_sq.finishSquare) {
				if(PrintSubTexture(renderer, jetons, x_actuel, y_actuel, 
								old_sq.finishColor, old_sq.finishElement, 
								sq_size - 4) != 0) {
					return -1;
				}
			}

			//Copie de la tête du chemin de droite dans actuel
			(*actuel).positions[k] = p;
			actuel->dist++;
		}		
	}
	//Suppression de la tête du chemin de droite 
		Chemin* suivantDroite = z->droite->suivant;
		free(z->droite);
		z->droite = suivantDroite;

	return 0;	
}


/*****************************INIT*******************************/

int InitializeRoboRicochet(GameStruct* g, char* id) {
	//Initialiser différents plateaux selon les valeurs de id (nombres)

	if (!SDL_strcmp(id, "r")) {
		if (SetupEmptyBoard(g->renderer) < 0 ||
		SetupRandomJetonsAndWalls(g->renderer, g->jetons, g->positionsJetons, g->board, 
								  g->horizontalWalls, g->verticalWalls) < 0 ||
		SetupRandomRobots(g->renderer, g->robots, g->board, &g->actuel) < 0) {
			return -1;
		}
	} else {
		if (SetupEmptyBoard(g->renderer) < 0 ||
		SetupNormalJetonsAndWalls(g->renderer, g->jetons, g->positionsJetons, g->board, g->horizontalWalls, g->verticalWalls) < 0 ||
		SetupNormalRobots(g->renderer, g->robots, &g->actuel, g->board) < 0) {
			return -1;
		}
		
	} 
	return AfficherBouton(g->renderer, &g->startButton);
}

int SetupEmptyBoard(SDL_Renderer* renderer) {
	SetColor(renderer, white);
	SDL_RenderClear(renderer);
	for (int i=0; i<BOARD_SIZE; i++) {
		for (int j=0; j<BOARD_SIZE; j++) {
			SDL_Rect rect = {x_offset + i*sq_size, y_offset +j*sq_size, sq_size, sq_size};
			SetColor(renderer, beige);
			if (SDL_RenderFillRect(renderer, &rect) < 0) return -1;
			SetColor(renderer, darkGrey);
			if (SDL_RenderDrawRect(renderer, &rect) < 0) return -1;
		}
	}
	SetColor(renderer, black);
	SDL_Rect centre = {x_offset + (milieu - 1)*sq_size, y_offset + (milieu - 1)*sq_size, 2*sq_size, 2*sq_size};
	return SDL_RenderFillRect(renderer, &centre);
}

/*****************************SETUP NORMAL*************************************/

int SetupNormalJetonsAndWalls(SDL_Renderer* renderer, dimTexture* jetons, Point positionsJetons[16], Square board[BOARD_SIZE][BOARD_SIZE],
			bool horizontalWalls[BOARD_SIZE][BOARD_SIZE + 1], bool verticalWalls[BOARD_SIZE][BOARD_SIZE + 1]) {
	/*******************Initialisation********************/
	for (int k=0; k<BOARD_SIZE; k++) {
		for (int m=0; m<BOARD_SIZE + 1; m++) {
			horizontalWalls[k][m] = false;
			verticalWalls[k][m] = false;
		}
	}
	
	for (int i=0; i<BOARD_SIZE; i++) {
		for (int j=0; j<BOARD_SIZE; j++) {
			board[i][j].finishSquare = false;
		}
	}

	/*******************Murs Extérieurs********************/
	for (int k=0; k<BOARD_SIZE + 1; k++) {
 		verticalWalls[k][0] = true;
 		verticalWalls[k][BOARD_SIZE] = true;
	}
	for (int k=0; k<BOARD_SIZE; k++) {
	 	horizontalWalls[k][0] = true;
		horizontalWalls[k][BOARD_SIZE] = true;
	}

	/*******************Centre********************/
	
	int centre[] = {milieu - 1, milieu + 1};
	for (int m=0; m<2; m++) {
		verticalWalls[milieu - 1][centre[m]] = true;
		verticalWalls[milieu][centre[m]] = true;
		
		horizontalWalls[milieu - 1][centre[m]] = true;
		horizontalWalls[milieu][centre[m]] = true;
	}

	/******************Murs*********************/
	const Point vert[] = {
		{0, 4}, {0, 10},
		{1, 14},
		{2, 6},
		{3, 9},
		{4, 3}, {4, 15},
		{5, 7},
		{6, 1}, {6, 12},
		{9, 4}, {9, 13},
		{10, 6}, 
		{11, 10},
		{12, 8},
		{13, 2}, {13, 15},
		{14, 4}, {14, 10},
		{15, 5}, {15, 12}
	};

	const Point horiz[] = {
		{0, 5}, {0, 11},
		{1, 6}, {1, 13},
		{2, 4},
		{3, 15},
		{4, 10},
		{5, 3},
		{6, 10},
		{7, 6}, {7, 12},
		{9, 3}, {9, 12},
		{10, 14},
		{12, 7}, 
		{13, 2}, {13, 10},
		{14, 4}, {14, 13},
		{15, 2}, {15, 12}
	};
	const int length = 21;

	for (int k=0; k<length; k++) {
		horizontalWalls[horiz[k].i][horiz[k].j] = true;
		verticalWalls[vert[k].i][vert[k].j] = true;
	}
	/*******************Affichage des jetons******************/

	const Point place_jetons[] = {
		{5, 2}, {1, 13}, {10, 14}, {12, 6},
		{9, 3}, {14, 13}, {3, 14}, {1, 6},
		{13, 9}, {7, 5}, {13, 1}, {4, 9},
		{6, 10}, {14, 4}, {2, 4}, {9, 11},
		{7, 12}
	};

	for (int k=0; k<16; k++) {
		positionsJetons[k] = place_jetons[k];
	}
	
	for (int k=0; k<17; k++) {
		Point p = place_jetons[k];
		int x = x_offset + p.i*sq_size;
		int y = y_offset + p.j*sq_size;
		board[p.i][p.j].finishSquare = true;
		board[p.i][p.j].finishElement = k/4;
		board[p.i][p.j].finishColor = k%4;
		if(PrintSubTexture(renderer, jetons, x + 2, y + 2, k%4, k/4, sq_size - 4) < 0) return -1;
	}
		
	/*******************Affichage des murs********************/

	SetColor(renderer, black);
	for (int k=0; k<BOARD_SIZE; k++) {
		for (int m=0; m<BOARD_SIZE + 1; m++) {
			if (verticalWalls[k][m]) {
				if (PrintVerticalWall(renderer, k, m) < 0) return -1;
			}
			if (horizontalWalls[k][m]) {
				if (PrintHorizontalWall(renderer, k , m) < 0) return -1;
			}
		}
	}

	return 0;
}

int SetupNormalRobots(SDL_Renderer* renderer, dimTexture* robots, Sommet* actuel, Square board[BOARD_SIZE][BOARD_SIZE]) {
	SDL_Color sb_colors[] = {sb_blue, sb_red, sb_green, sb_yellow};
	//INITIALISATION
	for (int i=0; i<BOARD_SIZE; i++) {
		for (int j=0; j<BOARD_SIZE; j++) {
			board[i][j].startingBlock = false;
		}
	}

	const Point robo_positions[] = {
		{1, 2}, {0, 0}, 
		{7, 4}, {12, 5}
	}; 

	for (int k=0; k<N_Robots; k++) {
		int i = robo_positions[k].i, j = robo_positions[k].j;
		int x = x_offset + i*sq_size;
		int y = y_offset + j*sq_size;
		actuel->positions[k] = robo_positions[k];
		board[i][j].startingBlock = true;
		board[i][j].startColor = sb_colors[k];
		DrawStartingBlock(renderer, i, j, sb_colors[k]);
		PrintSubTexture(renderer, robots, x + 2, y + 2, k, 0, sq_size - 4);
	}
	
	return 0;
}


/*****************************SETUP RANDOM*************************************/

int SetupRandomJetonsAndWalls(SDL_Renderer* renderer, dimTexture* jetons, Point positionJetons[16], Square board[BOARD_SIZE][BOARD_SIZE],
			bool horizontalWalls[BOARD_SIZE][BOARD_SIZE + 1], bool verticalWalls[BOARD_SIZE][BOARD_SIZE + 1]) {
	
	/*******************Initialisation********************/
	for (int k=0; k<BOARD_SIZE; k++) {
		for (int m=0; m<BOARD_SIZE + 1; m++) {
			horizontalWalls[k][m] = false;
			verticalWalls[k][m] = false;
		}
	}
	
	for (int i=0; i<BOARD_SIZE; i++) {
		for (int j=0; j<BOARD_SIZE; j++) {
			board[i][j].finishSquare = false;
		}
	}

	/*******************Murs Extérieurs********************/
	for (int k=0; k<BOARD_SIZE + 1; k++) {
 		verticalWalls[k][0] = true;
 		verticalWalls[k][BOARD_SIZE] = true;
	}
	for (int k=0; k<BOARD_SIZE; k++) {
	 	horizontalWalls[k][0] = true;
		horizontalWalls[k][BOARD_SIZE] = true;
	}

	/*******************Murs sur les côtés*****************/
	//0 = Gauche; 1 = Haut; 2 = Droite; 3 = Bas
	int m[4] = {0, 0, BOARD_SIZE-1, BOARD_SIZE - 1};
	
	for(int c=0; c<4; c++) {
		int num1 = RandomInt(1, BOARD_SIZE - 1);
		int num2 = RandomInt(1, BOARD_SIZE - 1);
		if (c%2 == 0) {
			horizontalWalls[m[c]][num1] = true;
			horizontalWalls[m[c]][num2] = true;
		} else {
			verticalWalls[m[c]][num1] = true;
			verticalWalls[m[c]][num2] = true;
		}
	}

	/*******************Centre********************/
	
	int centre[] = {milieu - 1, milieu + 1};
	for (int m=0; m<2; m++) {
		verticalWalls[milieu - 1][centre[m]] = true;
		verticalWalls[milieu][centre[m]] = true;
		
		horizontalWalls[milieu - 1][centre[m]] = true;
		horizontalWalls[milieu][centre[m]] = true;
	}

	/******************Jetons*********************/

	for (int k=0; k<4; k++) {
		for (int l=0; l<4; l++) {
			int i, j;
			RandomPositionPasCentre(&i, &j);
			int max = 0;
			while (board[i][j].finishSquare && max < 10000) { 
				//No two symbols on same square
				RandomPositionPasCentre(&i, &j);
				max++;
			}
			if (max > 9998) return -42;
			positionJetons[4*k + l] = (Point) {i, j};
			board[i][j].finishSquare = true;
			board[i][j].finishElement = k;
			board[i][j].finishColor = l;

			/****Murs autour du jeton********/
			int dj = RandomInt(0, 1); 
			int di = RandomInt(0, 1); 
			horizontalWalls[i][j + dj] = true;
			verticalWalls[j][i + di] = true;
			/********************************/

			int x = x_offset + i*sq_size;
			int y = y_offset + j*sq_size;
			PrintSubTexture(renderer, jetons, x + 2, y + 2, l, k, sq_size - 4);
		}
	}
		
	/*******************Affichage des murs********************/

	SetColor(renderer, black);
	for (int k=0; k<BOARD_SIZE; k++) {
		for (int m=0; m<BOARD_SIZE + 1; m++) {
			if (verticalWalls[k][m]) {
				if (PrintVerticalWall(renderer, k, m) < 0) return -1;
			}
			if (horizontalWalls[k][m]) {
				if (PrintHorizontalWall(renderer, k , m) < 0) return -1;
			}
		}
	}
	
	return 0;
}


int SetupRandomRobots(SDL_Renderer* renderer, dimTexture* robots, Square board[BOARD_SIZE][BOARD_SIZE], Sommet* actuel) {
	SDL_Color sb_colors[] = {sb_blue, sb_red, sb_green, sb_yellow};
	//INITIALISATION
	for (int i=0; i<BOARD_SIZE; i++) {
		for (int j=0; j<BOARD_SIZE; j++) {
			board[i][j].startingBlock = false;
		}
	}


	for (int k=0; k<4; k++) {
		int i, j;
		RandomPositionPasCentre(&i, &j);

		int max = 0;
		while (board[i][j].finishSquare && board[i][j].startingBlock && max < 10000) {
			//No starting block on finish block or on other starting block
			RandomPositionPasCentre(&i, &j);
			max++;
		}
		actuel->positions[k] = (Point){i, j};
		if (max > 10000) return -42;
		board[i][j].startingBlock = true;
		board[i][j].startColor = sb_colors[k];
		int x = x_offset + i*sq_size;
		int y = y_offset + j*sq_size;
		if (DrawStartingBlock(renderer, i, j, sb_colors[k]) != 0 ||
		PrintSubTexture(renderer, robots, x + 2, y + 2, k, 0, sq_size - 4) != 0) return -1;
	}
	return 0;
}