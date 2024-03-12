#include "comparaison.h"
#include <stdio.h>

int PlateauRandomJetonsEtMurs (dimTexture* jetons, Point positionJetons[16], Square board[BOARD_SIZE][BOARD_SIZE],
							  bool horizontalWalls[BOARD_SIZE][BOARD_SIZE + 1], bool verticalWalls[BOARD_SIZE][BOARD_SIZE + 1]){

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

	for (int k=0; k<NB_JETONS; k++) {
		int i, j;
		RandomPositionPasCentre(&i, &j);
		int max = 0;
		while (board[i][j].finishSquare && max < 10000) {
			//No two symbols on same square
			RandomPositionPasCentre(&i, &j);
			max++;
		}
		if (max > 9998) return -42;
		positionJetons[k] = (Point) {i, j};
		board[i][j].finishSquare = true;
		board[i][j].finishElement = k/4;
		board[i][j].finishColor = k%4;

		/****Murs autour du jeton********/
		int dj = RandomInt(0, 1);
		int di = RandomInt(0, 1);
		horizontalWalls[i][j + dj] = true;
		verticalWalls[j][i + di] = true;
    }
    return 0;
}

int RobotsRandom (dimTexture* robots, Square board[BOARD_SIZE][BOARD_SIZE], Sommet* actuel){
    SDL_Color sb_colors[] = {sb_blue, sb_red, sb_green, sb_yellow};

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
	}
	return 0;
}

/*
void AfficherDonnees (Donnees* d, int n){
    for (int i = 0 ; i<n ; i++){
        printf("Plateau %d : %d coups\n", i, d[i].coups);
        printf("        - Dijksrta : %lf s \n",d[i].temps[0]);
        printf("        - Euclidienne : %lf s \n",d[i].temps[1]);
        printf("        - Manhattan : %lf s \n",d[i].temps[2]);
        printf("        - Heuristique 3 : %lf s \n",d[i].temps[3]); 
        printf("\n");
    }
}

void ImprimerDonees(FILE* flux, Donnees d, int k) {
	fprintf(flux, "%d;", d.coups);
	fprintf(flux, "%lf;", d.temps[0]);
    fprintf(flux, "%lf;",d.temps[1]);
    fprintf(flux, "%lf;",d.temps[2]);
    fprintf(flux, "%lf;",d.temps[3]);
	fprintf(flux, "\n");
}
*/

int comparaison (GameStruct* g, int n, FILE* flux){
    for (int i = 0; i<n; i++){
		printf("Plateau %d\n",i);
        // Plateau selection
        if (PlateauRandomJetonsEtMurs ( g->jetons, g->positionsJetons, g->board, g->horizontalWalls, g->verticalWalls) < 0 ||
			RobotsRandom (g->robots,g->board, &g->actuel) < 0) {
                return -1;
        }
		g->actuel.dist = 0;
		
        //Jeton selection
		int jeton;
		jeton = RandomInt(0, 16);
		Couleur c = jeton % 4;
		fprintf(flux, "Plateau %d;;",i);
		for (int k = 0; k < 100; k++){
			fprintf(flux, ";");
			printf("  ¤ Tour %d\n", k);
			for (int h = 0; h<NB_HEURISTIQUES; h++){
				FreeZipper(g->z);
				clock_t t1 = clock();
				switch (h) {
					case 0:
						g->z = Dijkstra(g->actuel, g->positionsJetons[jeton], c, g->horizontalWalls, g->verticalWalls);
						break;
					case 1:
						g->z = a_star(g->actuel, g->positionsJetons[jeton], c, h_euclidienne, g->horizontalWalls, g->verticalWalls);
						break;
					case 2:
						g->z = a_star(g->actuel, g->positionsJetons[jeton], c, h_manhattan, g->horizontalWalls, g->verticalWalls);
						break;	
					case 3:
						g->z = a_star(g->actuel, g->positionsJetons[jeton], c, yolo, g->horizontalWalls, g->verticalWalls);
						break;
				}
				clock_t t2 = clock();
				if (h==0){fprintf(flux, "%d;", g->z.distance_totale);}
				fprintf(flux, "%lf;", (double)(t2 - t1) / CLOCKS_PER_SEC);
				g->actuel.dist = 0;	
				printf("    - heuristique %d\n", h);
			}
			fprintf(flux, "%d;", g->z.distance_totale);
			fprintf(flux, "\n");
		}
		fprintf(flux, "\n");
    }
	return 0;
}

int recuperer (GameStruct* g, char* file){
	/* prendre les données et les ajouter à un fichier .txt */
	
	FILE* flux = fopen(file, "a"); // on passe en 1er argument le nom du fichier
	assert(flux != NULL);

	int n;
	while(scanf("%d", &n) != 1) {
		while(getchar());
	}

	Donnees* d = malloc(sizeof(Donnees)*n);
	assert(d!=NULL);

	if (comparaison(g, n, flux) != 0) return -1;

	fclose(flux);
	return 0;
}
