#include "comparaison.h"

/********************************************************

Faire une fonction pour comparer les heuristiques (choix des heuristiques?)
sur un nombre de plateaux prédéfinis (scanf + for)
avec le même item à chercher sur le même tableau et on change de tableau à chaque boucle

TO DO :
    - vérifier les structures utilisées 
    - faire une fonction "print", "free"...
    - free d dans main.c
    - tester la fonction

*********************************************************/
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

int comparaison (GameStruct* g, Donnees* d){
    int n ;
    scanf("nombre d'iterations:%d",&n);

    if (d != NULL) free (d);
    d = malloc(n*sizeof(Donnees));
    Donnees res;

    for (int i = 0; i<n; i++){
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
        
        for (int h = 0; h<NB_HEURISTIQUES; h++){
            // g->choixH = h;
            clock_t t1 = clock();
	    	switch (h) {
                case 0:
                    g->z = Dijkstra(g->actuel, g->positionsJetons[jeton], c, g->horizontalWalls, g->verticalWalls);
                    d[i].coups = g->z.distance_totale;
                    break;
                case 1:
                    g->z = a_star(g->actuel, g->positionsJetons[jeton], c, h_euclidienne, g->horizontalWalls, g->verticalWalls);
                    break;
                case 2:
                    g->z = a_star(g->actuel, g->positionsJetons[jeton], c, h_manhattan, g->horizontalWalls, g->verticalWalls);
                    break;	
                case 3:
                    g->z = a_star(g->actuel, g->positionsJetons[jeton], c, h_3, g->horizontalWalls, g->verticalWalls);
                    break;
            }
            clock_t t2 = clock();
    		d[i].temps[h] =  (double)(t2 - t1) / CLOCKS_PER_SEC;
            if (g->z.distance_totale != d[i].coups) return -1;
      		g->actuel.dist = 0;
        }
    }
    return n;
}
// en cours :
int recuperer (GammeStruct* g, Donnees* d){
	/* prendre les données et les ajouter à un fichier .txt */
	char* fichier;
	scanf("%s", &fichier);
	FILE* flux = fopen(fichier, "a"); // on passe en 1er argument le nom du fichier

	fclose(flux);
	return 0;
}
/*
int main(int argc, char* argv[]) {
	FILE* flux = fopen(argv[1], "r"); // on passe en 1er argument le nom du fichier
	if (flux == NULL) printf("Erreur de lecture du fichier");
	else {
		char* famille = argv[2]; // on passe en 2nd argument un nom de famille
		char nom[50];
		char prenom[50]; // on prevoit de la place pour 49 caracteres max
		int age;
		printf("Famille %s :\n", famille);
		while (fscanf(flux, "%s %s %d", nom, prenom, &age) != EOF) {
			if (strcmp(famille, nom) == 0) { // le nom lu est le nom qu’on cherche
				printf("- %s (%d ans)\n", prenom, age);
			}
		}
	}
	fclose(flux);
	return 0;
}
*/
