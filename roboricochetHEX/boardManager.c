#include "boardManager.h"
#include <string.h>



#define WALLNUMBER 40

const Hex board[BOARD_SIZE][BOARD_SIZE];

const SDL_Color white = {255, 255, 255, 255};
const SDL_Color black = {0, 0, 0, 255};
const SDL_Color darkGrey = {100, 100, 100, 255};
const SDL_Color beige = {227,212,173, 255};

const SDL_Color sb_red = {160, 0, 5, 150};
const SDL_Color sb_blue = {23, 78, 134, 150};
const SDL_Color sb_green = {0, 127, 12, 150};
const SDL_Color sb_yellow = {240, 244, 0, 150};

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
	const int hexSemiHeight = HEXSIZE * SDL_sqrt(3);
	*x = (2*q + r- BOARD_RADIUS + 1)*hexSemiHeight + x_offset;
	*y = 3*r*HEXSIZE + y_offset;
}

void GetPrintingCoordinates(int q, int r, int* x, int* y) {
	const int hexSemiHeight = HEXSIZE * SDL_sqrt(3);
	*x = (2*q + r- BOARD_RADIUS)*hexSemiHeight + x_offset + HEXSIZE/2;
	*y = 3*r*HEXSIZE + y_offset + hexSemiHeight/2;
}

int DrawVerticalWall(SDL_Renderer* renderer, int q_right, int r) {
	const int hexSemiHeight = HEXSIZE*SDL_sqrt(3);
	SetColor(renderer, black);
	SDL_Rect rect = {x_offset + (2*(q_right) + r - BOARD_RADIUS)*hexSemiHeight - 1, y_offset + (3*r + 1)*HEXSIZE, 3, 2*HEXSIZE + 1};
	return SDL_RenderFillRect(renderer, &rect);
}

int DrawDiagUpWall(SDL_Renderer* renderer, int q, int r_bot) {
	const int hexSemiHeight = HEXSIZE*SDL_sqrt(3);

	
	int x_down = (2*q + r_bot-BOARD_RADIUS)*hexSemiHeight + x_offset;
	int y_down = (3*r_bot + 1)*HEXSIZE + y_offset;

	int x_up = x_down + hexSemiHeight;
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
	const int hexSemiHeight = HEXSIZE*SDL_sqrt(3);

	int x_up, y_up; 
	GetTopCoordinates(q_top-1, r_bot, &x_up, &y_up);

	int x_down = x_up + hexSemiHeight;
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

int DrawHexagonFromTop (SDL_Renderer* renderer, const int top_x, const int top_y, const int hexSemiHeight) {
	SDL_Point points[7] = {
		{top_x, top_y}, 
		{top_x + hexSemiHeight, top_y + HEXSIZE}, 
		{top_x + hexSemiHeight, top_y + 3*HEXSIZE},
		{top_x, top_y + 4*HEXSIZE},
		{top_x - hexSemiHeight, top_y + 3*HEXSIZE},
		{top_x - hexSemiHeight, top_y + HEXSIZE},
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
}

int DrawCenter(SDL_Renderer* renderer, bool VerticalWalls[BOARD_SIZE+1][BOARD_SIZE+1],
		bool DiagupWalls[BOARD_SIZE+1][BOARD_SIZE+1],
		bool DiagDownWalls[BOARD_SIZE+1][BOARD_SIZE+1]) {
	const int hexSemiHeight = HEXSIZE * SDL_sqrt(3);

	int x, y;
	int dq_centre[7] = {-1,-1, 0, 0, 0, 1, 1};
	int dr_centre[7] = {0, 1, 1, 0, -1, -1, 0};
	for(int l=0; l<7; l++) {
		GetTopCoordinates(BOARD_RADIUS + dq_centre[l], BOARD_RADIUS + dr_centre[l], &x, &y);
		FillHexagonFromTop(renderer, x, y, hexSemiHeight, HEXSIZE);
	}


}

/**************************************************************************/

int SetupEmptyBoard(SDL_Renderer* renderer) {
	const int hexSemiHeight = HEXSIZE * SDL_sqrt(3);
	SetColor(renderer, white);
	SDL_RenderClear(renderer);
	
	
	int max = BOARD_RADIUS + 1;

	int top_y = y_offset;
	int Top_left_x = x_offset + (BOARD_RADIUS + 1)*hexSemiHeight;

	for (int i=0; i<=2*BOARD_RADIUS; i++) {
		int top_x = Top_left_x;
		for (int j=0; j<max; j++) {
			SetColor(renderer, beige);
			if (FillHexagonFromTop(renderer, top_x, top_y,  hexSemiHeight, HEXSIZE) != 0) return -1;
			SetColor(renderer, darkGrey);
			if (DrawHexagonFromTop(renderer, top_x, top_y, hexSemiHeight) != 0) return -1;
			top_x = top_x + 2*hexSemiHeight;
		}

		if(i < BOARD_RADIUS) {
			Top_left_x = Top_left_x - hexSemiHeight;
			max = max + 1;
		} else {
			Top_left_x = Top_left_x + hexSemiHeight;
			max = max - 1;
		}
		top_y = top_y + 3*HEXSIZE;
	}
	return 0;
}

int CheckeMurs(SDL_Renderer* renderer, bool VerticalWalls[BOARD_SIZE+1][BOARD_SIZE+1],
		bool DiagupWalls[BOARD_SIZE+1][BOARD_SIZE+1],
		bool DiagDownWalls[BOARD_SIZE+1][BOARD_SIZE+1]) {
	for(int q=0; q<=BOARD_SIZE; q++) {
		for(int r=0; r<=BOARD_SIZE; r++) {
			if (VerticalWalls[q][r]) DrawVerticalWall(renderer, q, r);
			if (DiagupWalls[q][r]) DrawDiagUpWall(renderer, q, r);
			if (DiagDownWalls[q][r]) DrawDiagDownWall(renderer, q, r);
		}
	}
}


/**************************INIT******************************/

int InitializeRoboRicochet(GameStruct* g, char* id) {
	
	if (SetupEmptyBoard(g->renderer) != 0) {
		return -1;
	}
	if(!strcmp(id, "r")) {
		if (SetupRandomJetonsAndWalls(g->renderer, g->jetons, g->board, g->positionJetons, g->VerticalWalls, g->DiagupWalls, g->DiagDownWalls) != 0 ||
		SetupRandomRobots(g->renderer, g->robots, g->board) != 0) return -1;
	} else if (!strcmp(id, "d")) {
		CheckeMurs(g->renderer, g->VerticalWalls, g->DiagupWalls, g->DiagDownWalls);
	} else {
		SetupNormalJetonsAndWalls(g->renderer, g->jetons, g->board, g->positionJetons, g->VerticalWalls, g->DiagupWalls, g->DiagDownWalls);
	}
	return 0;
}


/*************************SETUP NORMAL************************/

int SetupNormalJetonsAndWalls(SDL_Renderer *renderer, dimTexture *jetons, Hex board[BOARD_SIZE][BOARD_SIZE], Point positionJetons[16],
		bool VerticalWalls[BOARD_SIZE+1][BOARD_SIZE+1],
		bool DiagupWalls[BOARD_SIZE+1][BOARD_SIZE+1],
		bool DiagDownWalls[BOARD_SIZE+1][BOARD_SIZE+1]) {


	/**************RESET DES TABLEAUX***********/
	for(int q=0; q<=BOARD_SIZE; q++) {
		for(int r=0; r<=BOARD_SIZE; r++) {
			VerticalWalls[q][r] = false;
			DiagupWalls[q][r] = false;
			DiagDownWalls[q][r] = false;
		}
	}

	const int hexSemiHeight = HEXSIZE * SDL_sqrt(3);

	/*Murs Extérieurs*/
	DrawOutsideWalls(renderer, VerticalWalls, DiagupWalls, DiagDownWalls);

	/*Centre*/
	int x, y;
	int dq_centre[7] = {-1,-1, 0, 0, 0, 1, 1};
	int dr_centre[7] = {0, 1, 1, 0, -1, -1, 0};
	for(int l=0; l<7; l++) {
		GetTopCoordinates(BOARD_RADIUS + dq_centre[l], BOARD_RADIUS + dr_centre[l], &x, &y);
		FillHexagonFromTop(renderer, x, y, hexSemiHeight, HEXSIZE);
	}

	int q_vertical_walls[40] = {};
	int r_vertical_walls[40] = {};

	

	return 0;
}

/**********************SETUP RANDOM*********************/

int SetupRandomJetonsAndWalls(SDL_Renderer *renderer, dimTexture *jetons, Hex board[BOARD_SIZE][BOARD_SIZE], Point positionsJetons[17],
		bool VerticalWalls[BOARD_SIZE+1][BOARD_SIZE+1],
		bool DiagupWalls[BOARD_SIZE+1][BOARD_SIZE+1],
		bool DiagDownWalls[BOARD_SIZE+1][BOARD_SIZE+1]) {
	const int hexSemiHeight = HEXSIZE * SDL_sqrt(3);

	//bool Walls[3][BOARD_SIZE+1][BOARD_SIZE+1] = {VerticalWalls, DiagupWalls, DiagDownWalls};

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
	DrawCenter(renderer, VerticalWalls, DiagupWalls, DiagDownWalls);

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
		board[p.q][p.r].c = k%4;
		board[p.q][p.r].e = k/4;
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
			while (max < 10000 && alreadyAWallThere(VerticalWalls, DiagupWalls, DiagDownWalls, cote%3, q, r)) {
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


int SetupRandomRobots(SDL_Renderer* renderer, dimTexture* robots, Hex board[BOARD_SIZE][BOARD_SIZE]) {

	SDL_Color sb_colors[] = {sb_blue, sb_red, sb_green, sb_yellow};
	/*******INITIALIZATION******/
	for(int q=0; q<BOARD_SIZE; q++) {
		for(int r=0; r<BOARD_SIZE; r++) {
			board[q][r].startingBlock = false;
		}
	}
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
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	int x, y;
	for (int i=0; i<N_Robots; i++) {
		Point p = RandomPoint(r_min, r_max);
		while (board[p.q][p.r].finishHex || board[p.q][p.r].startingBlock) {
			p = RandomPoint(r_min, r_max);
		}
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