#include "boardManager.h"
#include <assert.h>
#include <string.h>





const int x_offset = 20;
const int y_offset = 10;

const int printing_size = HEXSIZE*2 + HEXSIZE/2;


void SetColor (SDL_Renderer* renderer, SDL_Color color) {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

int RandomInt(int min, int max) {
	int mod = SDL_abs(max - min) + 1;
	return rand() % mod + min;
}

int RandomUnion (int min1, int max1, int min2, int max2) {
	int r1 = RandomInt(min1, max1);
	int r2 = RandomInt(min2, max2);
	int u = RandomInt(0, 1);

	if (u == 1) return r1;

	return r2;
}

Point RandomPoint(const int r_min[BOARD_SIZE], const int r_max[BOARD_SIZE]) {
	Point p;
	p.q = rand() % BOARD_SIZE;
	switch(p.q) {
		case (BOARD_RADIUS - 1):
			p.r = RandomUnion(r_min[p.q], BOARD_RADIUS - 1, BOARD_RADIUS + 2, r_max[p.q]);
		break;
		case (BOARD_RADIUS):
			p.r = RandomUnion(r_min[p.q], BOARD_RADIUS - 2, BOARD_RADIUS + 2, r_max[p.q]);
		break;
		case (BOARD_RADIUS + 1):
			p.r = RandomUnion(r_min[p.q], BOARD_RADIUS - 2, BOARD_RADIUS + 1, r_max[p.q]);
		break;
		default : 
			p.r = RandomInt(r_min[p.q], r_max[p.q]);
		break;
	}

	return p;
}

bool alreadyAWallThere(bool VerticalWalls[BOARD_SIZE+1][BOARD_SIZE+1], 
					   bool DiagupWalls[BOARD_SIZE+1][BOARD_SIZE+1],
					   bool DiagDownWalls[BOARD_SIZE+1][BOARD_SIZE+1], WallType wt, int q, int r) {
	switch (wt) {
		case VERTICAL: return VerticalWalls[q][r]; break;
		case DIAGUP: return DiagupWalls[q][r]; break;
		case DIAGDOWN: return DiagDownWalls[q][r]; break;
		default: break;
	}
	return false;
}

void GetTopCoordinates(int q, int r, int* x, int* y) {
	*x = (2*q + r- BOARD_RADIUS + 1)*HEXSEMIHEIGHT + x_offset;
	*y = 3*r*HEXSIZE + y_offset;
}

void GetPrintingCoordinates(int q, int r, int* x, int* y) {
	*x = (2*q + r- BOARD_RADIUS)*HEXSEMIHEIGHT + x_offset + HEXSIZE/2;
	*y = 3*r*HEXSIZE + y_offset + HEXSEMIHEIGHT/2;
}

int DrawVerticalWall(SDL_Renderer* renderer, int q_right, int r) {
	SetColor(renderer, black);
	SDL_Rect rect = {x_offset + (2*(q_right) + r - BOARD_RADIUS)*HEXSEMIHEIGHT - 1, y_offset + (3*r + 1)*HEXSIZE, 3, 2*HEXSIZE + 1};
	return SDL_RenderFillRect(renderer, &rect);
}

int DrawDiagUpWall(SDL_Renderer* renderer, int q, int r_bot) {
	
	
	int x_down = (2*q + r_bot-BOARD_RADIUS)*HEXSEMIHEIGHT + x_offset;
	int y_down = (3*r_bot + 1)*HEXSIZE + y_offset;

	int x_up = x_down + HEXSEMIHEIGHT;
	int y_up = y_down - HEXSIZE;
	SetColor(renderer, black);

	for(int k=0; k<3; k++) {
		if (SDL_RenderDrawLine(renderer, x_down, y_down - k, x_up, y_up - k) != 0 ||
			SDL_RenderDrawLine(renderer, x_down + k, y_down, x_up + k, y_up) != 0) {
			return -1;
		}
	}
	return 0;
}

int DrawDiagDownWall(SDL_Renderer* renderer, int q_top, int r_bot) {
	int x_up, y_up; 
	GetTopCoordinates(q_top-1, r_bot, &x_up, &y_up);

	int x_down = x_up + HEXSEMIHEIGHT;
	int y_down = y_up + HEXSIZE;
	SetColor(renderer, black);

	
	for(int k=0; k<3; k++) {
		if (SDL_RenderDrawLine(renderer, x_down - k, y_down, x_up - k, y_up) != 0 ||
			SDL_RenderDrawLine(renderer, x_down, y_down - k, x_up, y_up - k) != 0) {
			return -1;
		}
	}
	return 0;
}

int DrawWall(SDL_Renderer* renderer, int q, int r, WallType wt) {
	switch(wt) {
		case VERTICAL: return DrawVerticalWall(renderer, q, r);
		break;
		case DIAGUP: return DrawDiagUpWall(renderer, q, r);
		break;
		case DIAGDOWN: return DrawDiagDownWall(renderer, q, r);
	}
}

int DrawHexagonFromTop (SDL_Renderer* renderer, const int top_x, const int top_y, const int hexSemiHeight, const int hexSize) {
	SDL_Point points[7] = {
		{top_x, top_y}, 
		{top_x + hexSemiHeight, top_y + hexSize}, 
		{top_x + hexSemiHeight, top_y + 3*hexSize},
		{top_x, top_y + 4*hexSize},
		{top_x - hexSemiHeight, top_y + 3*hexSize},
		{top_x - hexSemiHeight, top_y + hexSize},
		{top_x, top_y}};

	
	return SDL_RenderDrawLines(renderer, points, 7);
}

int FillHexagonFromTop (SDL_Renderer* renderer, int top_x, int top_y, const int hexSemiHeight, const int size) {
	int bot_y = top_y + 4*size;

	
	//Top part
	for (int i=0; i<size; i++) {
		int length = (int) i*SDL_tan(M_PI / 3);
		if (SDL_RenderDrawLine(renderer, top_x - length, top_y + i, top_x + length, top_y + i) != 0) {
			return -1;
		}
	}

	//Middle Rectangle
	SDL_Rect central = {top_x - hexSemiHeight, top_y + size, 2*hexSemiHeight, 2*size};
	int check3 = SDL_RenderFillRect(renderer, &central);

	//Right Part
	for (int i=0; i<size+1; i++) {
		double l = i*SDL_tan(M_PI / 3);
		int length = (int) l;
		if (SDL_RenderDrawLine(renderer, top_x - length, bot_y - i, top_x + length, bot_y - i) != 0) {
			return -1;
		}
	}

	if (check3) return -1;

	return 0;
}


int FillCenterHexagon(SDL_Renderer* renderer) {
	int top_x, top_y;
	GetTopCoordinates(BOARD_RADIUS, BOARD_RADIUS, &top_x, &top_y);
	return FillHexagonFromTop(renderer, top_x, top_y, HEXSEMIHEIGHT, HEXSIZE);
}

int DrawOutsideWalls(SDL_Renderer* renderer, bool VerticalWalls[BOARD_SIZE+1][BOARD_SIZE+1],
		bool DiagupWalls[BOARD_SIZE+1][BOARD_SIZE+1],
		bool DiagDownWalls[BOARD_SIZE+1][BOARD_SIZE+1]) {
	int q = 0;
	int r = BOARD_RADIUS;
	for(int k=0; k<=BOARD_RADIUS; k++) {
		if (DrawVerticalWall(renderer, q, r) != 0) return -1;
		VerticalWalls[q][r] = true;
		if (DrawDiagUpWall(renderer, q, r) != 0) return -1;
		DiagupWalls[q][r] = true;
		q++;
		r--;
	}
	r++;
	for(int k=0; k<=BOARD_RADIUS-1; k++) {
		if (DrawDiagUpWall(renderer, q, r) != 0) return -1;
		DiagupWalls[q][r] = true;
		if (DrawDiagDownWall(renderer, q, r) != 0) return -1;
		DiagDownWalls[q][r] = true;
		q++;
	}
	
	for(int k=0; k<=BOARD_RADIUS; k++) {
		if (DrawDiagDownWall(renderer, q, r) != 0) return -1;
		DiagDownWalls[q][r] = true;
		if (DrawVerticalWall(renderer, q, r) != 0) return -1;
		VerticalWalls[q][r] = true;
		r++; 
	}
	r--;
	for(int k=0; k<=BOARD_RADIUS; k++) {
		if (DrawVerticalWall(renderer, q, r) != 0) return -1;
		VerticalWalls[q][r] = true;
		r++;
		q--;
		if (DrawDiagUpWall(renderer, q, r) != 0) return -1;
		DiagupWalls[q][r] = true;
		
	}
	for(int k=0; k<=BOARD_RADIUS; k++) {
		if (DrawDiagUpWall(renderer, q, r) != 0) return -1;
		DiagupWalls[q][r] = true;
		if (DrawDiagDownWall(renderer, q, r) != 0) return -1;
		DiagDownWalls[q][r] = true;
		q--;
	}
	q++;
	for(int k=0; k<=BOARD_RADIUS; k++) {
		if (DrawDiagDownWall(renderer, q, r) != 0) return -1;
		DiagDownWalls[q][r] = true;
		r--;
		if (DrawVerticalWall(renderer, q, r) != 0) return -1;
		VerticalWalls[q][r] = true;
	}
	return 0;
}

int DrawCenter(SDL_Renderer* renderer) {
	int x, y;
	int dq_centre[7] = {-1,-1, 0, 0, 0, 1, 1};
	int dr_centre[7] = {0, 1, 1, 0, -1, -1, 0};
	for(int l=0; l<7; l++) {
		GetTopCoordinates(BOARD_RADIUS + dq_centre[l], BOARD_RADIUS + dr_centre[l], &x, &y);
		FillHexagonFromTop(renderer, x, y, HEXSEMIHEIGHT, HEXSIZE);
	}
	return 0;
}

int DrawJeton(SDL_Renderer* renderer, dimTexture* jetons, const int numero, Point position) {
	int x, y;
	GetPrintingCoordinates(position.q, position.r, &x, &y);
	if (PrintSubTexture(renderer, jetons, x, y, numero%4, numero/4, printing_size) != 0) return -1;
	
	return 0;
}

int DrawStartingBlock(SDL_Renderer* renderer, int q, int r, SDL_Color color) {
	SetColor(renderer, color);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	int x, y;
	GetTopCoordinates(q, r, &x, &y);
	if (FillHexagonFromTop(renderer, x, y+4, (HEXSIZE - 2)*SDL_sqrt(3), HEXSIZE - 2) != 0) return -1;
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
	return 0;
}

void CopieVisites(bool t1[BOARD_SIZE][BOARD_SIZE], bool t2[BOARD_SIZE][BOARD_SIZE]) {
	for(int k=0; k<BOARD_SIZE; k++) {
		for(int l=0; l<BOARD_SIZE; l++) {
			t2[k][l] = t1[k][l];
		}
	}
}

bool EgaliteVisites(bool t1[BOARD_SIZE][BOARD_SIZE], bool t2[BOARD_SIZE][BOARD_SIZE]) {
	for(int k=0; k<BOARD_SIZE; k++) {
		for(int l=0; l<BOARD_SIZE; l++) {
			if (t2[k][l] != t1[k][l]) return false;
		}
	}
	return true;
}

bool Connexe(Hex board[BOARD_SIZE][BOARD_SIZE], Point s, Point t, 
			 bool VerticalWalls[BOARD_SIZE+1][BOARD_SIZE+1],
			 bool DiagupWalls[BOARD_SIZE+1][BOARD_SIZE+1],
			 bool DiagDownWalls[BOARD_SIZE+1][BOARD_SIZE+1]) {
	
	bool visite[BOARD_SIZE][BOARD_SIZE];
	bool visite2[BOARD_SIZE][BOARD_SIZE];
	for(int q=0; q<BOARD_SIZE; q++) {
		for(int r=0; r<BOARD_SIZE; r++) {
			visite[q][r] = false;
			visite2[q][r] = false;
		}
	}
	
	visite2[s.q][s.r] = true;
	while (!EgaliteVisites(visite, visite2)) {
		CopieVisites(visite2, visite);
		if (visite[t.q][t.r]) return true;

		for(int q=0; q<BOARD_SIZE; q++) {
			for(int r=0; r<BOARD_SIZE; r++) {
				if (visite2[q][r]) {
					if (q>0 && !VerticalWalls[q][r]) { //Gauche
						visite2[q-1][r] = true;
					}
					if (r>0 && !DiagupWalls[q][r]) { //Haut Gauche
						visite2[q][r-1] = true;
					}
					if (q<BOARD_SIZE-1 && r>0 && !DiagDownWalls[q+1][r]) { //Haut Droite
						visite2[q+1][r-1] = true;
					}
					if (q<BOARD_SIZE-1 && !VerticalWalls[q+1][r]) { //Droite
						visite2[q+1][r] = true;
					}
					if(r<BOARD_SIZE-1 && !DiagupWalls[q][r+1]) { //Bas Droite
						visite2[q][r+1] = true;
					}
					if (r<BOARD_SIZE-1 && q>0 && !DiagDownWalls[q][r+1]) { //Bas Gauche
						visite2[q-1][r+1] = true;
					}
				}
			}
		}
	}
	return false;
}




/**************************************************************************/

int SetupEmptyBoard(SDL_Renderer* renderer) {
	SetColor(renderer, white);
	SDL_RenderClear(renderer);
	
	
	int max = BOARD_RADIUS + 1;

	int top_y = y_offset;
	int Top_left_x = x_offset + (BOARD_RADIUS + 1)*HEXSEMIHEIGHT;

	for (int i=0; i<=2*BOARD_RADIUS; i++) {
		int top_x = Top_left_x;
		for (int j=0; j<max; j++) {
			SetColor(renderer, beige);
			if (FillHexagonFromTop(renderer, top_x, top_y,  HEXSEMIHEIGHT, HEXSIZE) != 0) return -1;
			SetColor(renderer, darkGrey);
			if (DrawHexagonFromTop(renderer, top_x, top_y, HEXSEMIHEIGHT, HEXSIZE) != 0) return -1;
			top_x = top_x + 2*HEXSEMIHEIGHT;
		}

		if(i < BOARD_RADIUS) {
			Top_left_x = Top_left_x - HEXSEMIHEIGHT;
			max = max + 1;
		} else {
			Top_left_x = Top_left_x + HEXSEMIHEIGHT;
			max = max - 1;
		}
		top_y = top_y + 3*HEXSIZE;
	}
	return 0;
}

int CheckePosition(SDL_Renderer* renderer, Sommet actuel, dimTexture* robots,
		bool VerticalWalls[BOARD_SIZE+1][BOARD_SIZE+1],
		bool DiagupWalls[BOARD_SIZE+1][BOARD_SIZE+1],
		bool DiagDownWalls[BOARD_SIZE+1][BOARD_SIZE+1]) {
	for(int q=0; q<=BOARD_SIZE; q++) {
		for(int r=0; r<=BOARD_SIZE; r++) {
			if (VerticalWalls[q][r]) DrawVerticalWall(renderer, q, r);
			if (DiagupWalls[q][r]) DrawDiagUpWall(renderer, q, r);
			if (DiagDownWalls[q][r]) DrawDiagDownWall(renderer, q, r);
		}
	}
	int x, y;
	for(int i=0; i<N_Robots; i++) {
		GetPrintingCoordinates(actuel.positions[i].q, actuel.positions[i].r, &x, &y);
		if (PrintSubTexture(renderer, robots, x, y, i, 0, printing_size) != 0) return -1;		
	}
	return 0;
}


int DeplaceGauche(SDL_Renderer* renderer, dimTexture* robots, dimTexture* jetons, Sommet* actuel, Hex board[BOARD_SIZE][BOARD_SIZE], Zipper* z) {

	if (z->gauche == NULL) {
		return 0;
	}

	//Copie de actuel en tête de droite
		Chemin* new = malloc(sizeof(Chemin));
		assert(new != NULL);
		for (int k=0; k<N_Robots; k++) {
			new->positions[k] = actuel->positions[k];
		}
		new->suivant = z->droite;
		z->droite = new;
	

	for (int k=0; k<N_Robots; k++) {
		Point p = z->gauche->positions[k];
		if (actuel->positions[k].q != p.q || actuel->positions[k].r != p.r) {
			int x, y;
			GetPrintingCoordinates(p.q, p.r, &x, &y);
			Hex old_hex = board[actuel->positions[k].q][actuel->positions[k].r];
			int x_actuel, y_actuel;
			GetTopCoordinates(actuel->positions[k].q, actuel->positions[k].r, &x_actuel, &y_actuel);
			
			//Affiche le robot sur la nouvelle case
			if (PrintSubTexture(renderer, robots, x, y, k, 0, printing_size) != 0) return -1;		
		

			//Efface la case précédente
			SetColor(renderer, beige);
			if (FillHexagonFromTop(renderer, x_actuel, y_actuel+4, (HEXSIZE - 2)*SDL_sqrt(3), HEXSIZE - 2) != 0) return -1;

			//Affiche si besoin le jeton ou la case de départ sur l'ancienne case
			GetPrintingCoordinates(actuel->positions[k].q, actuel->positions[k].r, &x_actuel, &y_actuel);
			if (old_hex.startingBlock) {
				if (DrawStartingBlock(renderer, actuel->positions[k].q, actuel->positions[k].r, old_hex.startColor) != 0) {
					return -1;
				}
			}
			if (old_hex.finishHex) {
				if(PrintSubTexture(renderer, jetons, x_actuel, y_actuel, 
								old_hex.finishColor, old_hex.finishElement, 
								printing_size) != 0) {
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

int DeplaceDroite(SDL_Renderer *renderer, dimTexture *robots, dimTexture *jetons, Sommet *actuel, Hex board[BOARD_SIZE][BOARD_SIZE], Zipper *z) {

	if (z->droite == NULL) {
		return 0;
	}

	//Copie de actuel en tête de gauche
		Chemin* new = malloc(sizeof(Chemin));
		assert(new != NULL);
		for (int k=0; k<N_Robots; k++) {
			new->positions[k] = actuel->positions[k];
		}
		new->suivant = z->gauche;
		z->gauche = new;
	

	for (int k=0; k<N_Robots; k++) {
		Point p = z->droite->positions[k];
		if (actuel->positions[k].q != p.q || actuel->positions[k].r != p.r) {
			int x, y;
			GetPrintingCoordinates(p.q, p.r, &x, &y);
			Hex old_hex = board[actuel->positions[k].q][actuel->positions[k].r];
			int x_actuel, y_actuel;
			GetTopCoordinates(actuel->positions[k].q, actuel->positions[k].r, &x_actuel, &y_actuel);

			//Affiche le robot sur la nouvelle case
			if (PrintSubTexture(renderer, robots, x, y, k, 0, printing_size) != 0) return -1;

			//Efface la case précédente
			SetColor(renderer, beige);
			if (FillHexagonFromTop(renderer, x_actuel, y_actuel+4, (HEXSIZE - 2)*SDL_sqrt(3), HEXSIZE - 2) != 0) return -1;

			//Affiche si besoin le jeton ou la case de départ sur l'ancienne case
			GetPrintingCoordinates(actuel->positions[k].q, actuel->positions[k].r, &x_actuel, &y_actuel);
			if (old_hex.startingBlock) {
				if (DrawStartingBlock(renderer, actuel->positions[k].q, 
						actuel->positions[k].r, old_hex.startColor) != 0) {
					return -1;
				}
			}
			if (old_hex.finishHex) {
				if(PrintSubTexture(renderer, jetons, x_actuel, y_actuel, 
								old_hex.finishColor, old_hex.finishElement, 
								printing_size) != 0) {
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


/**************************INIT******************************/

int InitializeRoboRicochet(GameStruct* g, char* id) {
	
	

	if (SetupEmptyBoard(g->renderer) != 0) {
		return -1;
	}
	
	if(!strcmp(id, "r")) {
		if (SetupRandomJetonsAndWalls(g->renderer, g->jetons, g->board, g->positionJetons, g->VerticalWalls, g->DiagupWalls, g->DiagDownWalls) != 0 ||
		SetupRandomRobots(g->renderer, g->robots, &g->actuel, g->board) != 0) return -1;
		FreeZipper(&g->z);
	} else if (!strcmp(id, "w")) {
		CheckePosition(g->renderer, g->actuel, g->robots, g->VerticalWalls, g->DiagupWalls, g->DiagDownWalls);
	} else {
		SetupNormalJetonsAndWalls(g->renderer, g->jetons, g->board, g->positionJetons, g->VerticalWalls, g->DiagupWalls, g->DiagDownWalls);
		SetupNormalRobots(g->renderer, g->robots, &g->actuel, g->board);
		FreeZipper(&g->z);
	}
	if (AfficherChoixHeuristiques(g->renderer, g->boutonsHeuristiques) != 0 
		|| AfficherChoixJetons(g->renderer,g->boutonsJetons) != 0
		|| AffichageHeuristiqueChoisie(g->renderer, g->choixH) != 0
		|| AffichageJetonChoisi(g->renderer, g->choixJ)) {
			return -1;
	}
	
	return AfficherBouton(g->renderer, &g->startButton);
}


/*************************SETUP NORMAL************************/

int SetupNormalJetonsAndWalls(SDL_Renderer *renderer, dimTexture *jetons, Hex board[BOARD_SIZE][BOARD_SIZE], Point positionJetons[16],
		bool VerticalWalls[BOARD_SIZE+1][BOARD_SIZE+1],
		bool DiagupWalls[BOARD_SIZE+1][BOARD_SIZE+1],
		bool DiagDownWalls[BOARD_SIZE+1][BOARD_SIZE+1]) {

	/*Initialization*/
	for(int q=0; q<BOARD_SIZE; q++) {
		for(int r=0; r<BOARD_SIZE; r++) {
			board[q][r].startingBlock = false;
			board[q][r].finishHex = false;
		}
	}
	

	/**************RESET DES TABLEAUX***********/
	for(int q=0; q<=BOARD_SIZE; q++) {
		for(int r=0; r<=BOARD_SIZE; r++) {
			VerticalWalls[q][r] = false;
			DiagupWalls[q][r] = false;
			DiagDownWalls[q][r] = false;
		}
	}

	
	/*Murs Extérieurs*/
	DrawOutsideWalls(renderer, VerticalWalls, DiagupWalls, DiagDownWalls);

	/*Centre*/
	int x, y;
	int dq_centre[7] = {-1,-1, 0, 0, 0, 1, 1};
	int dr_centre[7] = {0, 1, 1, 0, -1, -1, 0};
	for(int l=0; l<7; l++) {
		GetTopCoordinates(BOARD_RADIUS + dq_centre[l], BOARD_RADIUS + dr_centre[l], &x, &y);
		if (FillHexagonFromTop(renderer, x, y, HEXSEMIHEIGHT, HEXSIZE) != 0) return -1;
	}

	const Point vertical_walls[29] = {
		{12, 0},
		{18, 1},
		{12, 2},
		{8, 4},
		{5, 5}, {13, 5}, {16, 5}, {17, 5},
		{11, 6}, 
		{7, 8}, {9, 8}, {11, 8},
		{2, 9}, {3, 9}, {8, 9}, {11, 9}, {13, 9},
		{8, 10}, {10, 10},{17, 10},
		{12, 11},
		{4, 12},
		{7, 13},
		{11, 15},
		{6, 16}, {7, 16},
		{2, 17}, {3, 17},
		{5, 18}	
	};

	const Point diagonal_up_walls[29] = {
		{0, 14},
		{2, 10},
		{4, 12},
		{5, 6},
		{6, 9}, {6, 17},
		{7, 14},
		{8, 5}, {8, 9}, {8, 11},
		{9, 8}, {9, 11},
		{10, 6}, {10, 8}, {10, 10},
		{11, 2},
		{12, 10}, {12, 12},
		{13, 5},
		{16, 11},
		{17, 1},
		{18, 3}, {18, 6},
		{0, 0}, {0, 0}, {0, 0}, 
		{0, 0}, {0, 0}, {0, 0}
	};

	const Point diagonal_down_walls[29] = {
		//q_top+r_bot-s = 10
		{7, 3}, {2, 8}, //s=0
		{6, 5}, //s=1
		{8, 5}, //s=3
		{12, 2}, //s=4
		{6, 9}, //s=5
		{11, 6}, {5, 12}, //s=7
		{10, 8}, {8, 10}, //s=8
		{18, 1}, {13, 6}, {11, 8}, {8, 11}, //s=9
		{11, 9}, {9, 11}, {2, 18}, //s=10
		{8, 13}, //s=11
		{16, 6}, {12, 10}, //s=12
		{12, 12}, //s=14
		{11, 15}, //s=16
		{17, 10}, //s=17
		{16, 12}, {11, 17}, //s=18
		{0, 0}, {0, 0}, {0, 0},
		{0, 0}
	};

	Point p;		
	for(int i=0; i<29; i++) {
		p = vertical_walls[i];
		VerticalWalls[p.q][p.r] = true;
		DrawVerticalWall(renderer, p.q, p.r);

		p = diagonal_up_walls[i];
		DiagupWalls[p.q][p.r] = true;
		DrawDiagUpWall(renderer, p.q,  p.r);

		p = diagonal_down_walls[i];
		DiagDownWalls[p.q][p.r] = true;
		DrawDiagDownWall(renderer, p.q, p.r);
	}

	/*****************JETONS********************/
	Point posJetons[17] = {
		{6, 8}, {17, 1}, {5, 5}, {10, 6},
		{12, 9}, {2, 9}, {13, 5}, {4, 12},
		{6, 16}, {8, 4}, {12, 11}, {16, 10},
		{11, 2}, {10, 15}, {2, 17}, {7, 13},
		{16, 5}
	};

	for(int k=0; k<17; k++) {
		positionJetons[k] = posJetons[k];
		board[posJetons[k].q][posJetons[k].r].finishHex = true;
		board[posJetons[k].q][posJetons[k].r].finishColor = k%4;
		board[posJetons[k].q][posJetons[k].r].finishElement = k/4;	
		DrawJeton(renderer, jetons, k, positionJetons[k]);
	}
	return 0;
}

int SetupNormalRobots(SDL_Renderer* renderer, dimTexture* robots, Sommet* actuel, Hex board[BOARD_SIZE][BOARD_SIZE]) {
	SDL_Color sb_colors[] = {sb_blue, sb_red, sb_green, sb_yellow};
	

	Point posRobots[N_Robots] = {{7, 18}, {7, 5}, {2, 14}, {12, 6}};

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	int x, y;
	Point p;
	actuel->dist = 0;
	for (int i=0; i<N_Robots; i++) {
		p = posRobots[i];
		actuel->positions[i] = p;
		GetTopCoordinates(p.q, p.r, &x, &y);
		SetColor(renderer, sb_colors[i]);
		if (FillHexagonFromTop(renderer, x, y+4, (HEXSIZE - 2)*SDL_sqrt(3), HEXSIZE - 2) != 0) return -1;
		GetPrintingCoordinates(p.q, p.r, &x, &y);
		if (PrintSubTexture(renderer, robots, x, y, i, 0, printing_size) != 0) return -1;		
		board[p.q][p.r].startingBlock = true;
		board[p.q][p.r].startColor = sb_colors[i];
	}
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
	return 0;
}

/**********************SETUP RANDOM*********************/

int SetupRandomJetonsAndWalls(SDL_Renderer *renderer, dimTexture *jetons, Hex board[BOARD_SIZE][BOARD_SIZE], Point positionsJetons[17],
		bool VerticalWalls[BOARD_SIZE+1][BOARD_SIZE+1],
		bool DiagupWalls[BOARD_SIZE+1][BOARD_SIZE+1],
		bool DiagDownWalls[BOARD_SIZE+1][BOARD_SIZE+1]) {
	
	/*RESET BOARD*/
	for(int q=0; q<BOARD_SIZE; q++) {
		for(int r=0; r<BOARD_SIZE; r++) {
			board[q][r].finishHex = false;
			board[q][r].startingBlock = false;
		}
	}
	/**************RESET DES TABLEAUX***********/
	for(int q=0; q<=BOARD_SIZE; q++) {
		for(int r=0; r<=BOARD_SIZE; r++) {
			VerticalWalls[q][r] = false;
			DiagupWalls[q][r] = false;
			DiagDownWalls[q][r] = false;
		}
	}

	/*Murs Extérieurs*/
	DrawOutsideWalls(renderer, VerticalWalls, DiagupWalls, DiagDownWalls);

	/*Centre*/
	DrawCenter(renderer);

	const Point vertical_walls[6] = {
		{9, 8}, {11, 8},
		{8, 9}, {11, 9},
		{8, 10}, {10, 10}
	};
	
	const Point diagonal_up_walls[6] = {
		
		{8, 9}, {8, 11},
		{9, 8}, {9, 11},
		{10, 8}, {10, 10},
	};

	const Point diagonal_down_walls[6] = {
		//q_top+r_bot-s = 10
		{10, 8}, {8, 10}, //s=8
		{11, 8}, {8, 11}, //s=9
		{11, 9}, {9, 11} //s=10
	};

	Point p;
	for(int k=0; k<6; k++) {
		p = vertical_walls[k];
		VerticalWalls[p.q][p.r] = true;
		DrawVerticalWall(renderer, p.q, p.r);

		p = diagonal_up_walls[k];
		DiagupWalls[p.q][p.r] = true;
		DrawDiagUpWall(renderer, p.q, p.r);

		p = diagonal_down_walls[k];
		DiagDownWalls[p.q][p.r] = true;
		DrawDiagDownWall(renderer, p.q, p.r);
	}

	/*MURS SUR LE BORD*/
	for(int k=0; k<BOARD_RADIUS; k++) {
		int cote = rand() % 6;
		int numero = rand () % BOARD_RADIUS;
		Point positions_murs[6] = {
			{BOARD_RADIUS + numero + 1, 0}, {BOARD_SIZE - 1, numero + 1}, 
			{BOARD_SIZE-1-numero, BOARD_RADIUS+1+numero}, {numero+1, BOARD_SIZE - 1}, 
			{0, BOARD_SIZE-numero-1}, {numero+1, BOARD_RADIUS-numero}};
		int q = positions_murs[cote].q, r = positions_murs[cote].r;
		switch (cote%3) {
			case 0: VerticalWalls[q][r] = true; break;
			case 1: DiagupWalls[q][r] = true; break;
			case 2: DiagDownWalls[q][r] = true; break;
		}	
		DrawWall(renderer, q, r, cote%3);
	}

	/**************Jetons***************/
	//Initialization de r_min et r_max
	int r_min[BOARD_SIZE];
	int r_max[BOARD_SIZE];
	int u = BOARD_RADIUS;
	int v = BOARD_SIZE-1;
	for(int k=0; k<BOARD_SIZE; k++) {
		r_min[k] = u;
		r_max[k] = v;
		if (k < BOARD_RADIUS) {
			u--;
		} else {
			v--;
		}
	}

	int x, y;
	for(int k=0; k<17; k++) {
		Point p = RandomPoint(r_min, r_max);
		while (board[p.q][p.r].finishHex) { 
			p = RandomPoint(r_min, r_max);
		}
		board[p.q][p.r].finishHex = true;
		board[p.q][p.r].finishColor = k%4;
		board[p.q][p.r].finishElement = k/4;
		positionsJetons[k] = p;
		GetPrintingCoordinates(p.q, p.r, &x, &y);
		PrintSubTexture(renderer, jetons, x, y, k%4, k/4, printing_size);
	}
	
	/********PLACER DES MURS AUTOUR DES JETONS***************/
	for(int k=0; k<17; k++) {
		Point p = positionsJetons[k];
		Point contour[6] = {{p.q, p.r}, {p.q, p.r}, {p.q+1, p.r},
								 {p.q+1, p.r}, {p.q, p.r + 1}, {p.q, p.r + 1}};
		for(int l=0; l<3; l++) {
			int cote = RandomInt(0, 5);
			int q = contour[cote].q, r = contour[cote].r;
			int max = 0;
			while (max < 10000) { // && alreadyAWallThere(VerticalWalls, DiagupWalls, DiagDownWalls, cote%3, q, r)) {
				cote = RandomInt(0, 5);
				q = contour[cote].q;
				r = contour[cote].r;
				max++;
			}
			DrawWall(renderer, q, r, cote%3);
			switch (cote%3) {
				case 0: VerticalWalls[q][r] = true; break;
				case 1: DiagupWalls[q][r] = true; break;
				case 2: DiagDownWalls[q][r] = true; break;
			}
		}
	}
	return 0;
}


int SetupRandomRobots(SDL_Renderer* renderer, dimTexture* robots, Sommet* actuel, Hex board[BOARD_SIZE][BOARD_SIZE]) {

	SDL_Color sb_colors[] = {sb_blue, sb_red, sb_green, sb_yellow};
	


	//r_min et r_max
	int r_min[BOARD_SIZE];
	int r_max[BOARD_SIZE];
	int u = BOARD_RADIUS;
	int v = BOARD_SIZE-1;
	for(int k=0; k<BOARD_SIZE; k++) {
		r_min[k] = u;
		r_max[k] = v;
		if (k < BOARD_RADIUS) {
			u--;
		} else {
			v--;
		}
	}
	int x, y;
	actuel->dist = 0;
	for (int i=0; i<N_Robots; i++) {
		Point p = RandomPoint(r_min, r_max);
		while (board[p.q][p.r].finishHex || board[p.q][p.r].startingBlock) {
			p = RandomPoint(r_min, r_max);
		}
		actuel->positions[i] = p;
		
		DrawStartingBlock(renderer, p.q, p.r, sb_colors[i]);
		GetPrintingCoordinates(p.q, p.r, &x, &y);
		if (PrintSubTexture(renderer, robots, x, y, i, 0, printing_size) != 0) return -1;		
		board[p.q][p.r].startingBlock = true;
		board[p.q][p.r].startColor = sb_colors[i];
	}
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
	return 0;
}