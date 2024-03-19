#include "comparaison.h"
#include <assert.h>
#include <stdio.h>


int Reinitialiser(GameStruct* g, int* jeton, Couleur* c) {
	// Plateau selection
	if (SetupRandomJetonsAndWalls (g->renderer, g->jetons, g->board, g->positionJetons, g->VerticalWalls, g->DiagupWalls, g->DiagDownWalls) < 0 ||
		SetupRandomRobots (g->renderer, g->robots, &g->actuel, g->board) < 0) {
			return -1;
	}
	g->actuel.dist = 0;
		
	//Jeton selection
	*jeton = RandomInt(0, 16);
	*c = *jeton % 4;
	return 0;
}



void WallTabPrint(bool t[BOARD_SIZE+1][BOARD_SIZE+1]) {
	for(int k=0; k<=BOARD_SIZE; k++) {
		printf("	");
		for(int l=0; l<=BOARD_SIZE; l++) {
			if (t[k][l]) {printf(" . ");}
			else {printf(" | ");}
		}
		printf("\n");
	}
	printf("\n");
}

void FullGamestructPrint(GameStruct* g) {
	printf("Sommet actuel :\n");
	for(int i=0; i<N_Robots; i++) {
		printf("	- (%d, %d)\n", g->actuel.positions[i].q, g->actuel.positions[i].r);
	}
	printf("	- dist = %d\n", g->actuel.dist);
	
	printf("Choix :\n");
	printf("	- ChoixJ = %d\n", g->choixJ);
	printf("	- ChoixH = %d\n", g->choixH);

	printf("Tableaux de murs :\n");
	WallTabPrint(g->VerticalWalls);
	WallTabPrint(g->DiagupWalls);
	WallTabPrint(g->DiagDownWalls);

	printf("");
}

int comparaison (GameStruct* g, int n, FILE* flux) {
	for (int i = 0; i<n; i++){
	
		int plateau = (int) time(NULL);
		printf("Plateau %d\n",plateau);

		int jeton;
		Couleur c;
		Reinitialiser(g, &jeton, &c);

		while(!Connexe(g->board, g->actuel.positions[c], g->positionJetons[jeton], g->VerticalWalls, g->DiagupWalls, g->DiagDownWalls)) {
			printf(ANSI_COLOR_YELLOW "Warning : Non Connexe\n" ANSI_COLOR_RESET);
			Reinitialiser(g, &jeton, &c);
		}

		for (int k = 0; k < 100; k++){
			fprintf(flux, "%d;",plateau);
			printf(ANSI_COLOR_MAGENTA "  ¤ Tour %d\n" ANSI_COLOR_RESET, k);
			for (int h = 0; h<NB_HEURISTIQUES; h++){
				FreeZipper(&g->z);
				
				clock_t t1 = clock();
				switch (h) {
					case 0:
						g->z = Dijkstra(g->actuel, g->positionJetons[jeton], c, g->VerticalWalls, g->DiagupWalls, g->DiagDownWalls);
						break;
					case 1:
						g->z = a_star(g->actuel, g->positionJetons[jeton], c, h_euclidienne, g->VerticalWalls, g->DiagupWalls, g->DiagDownWalls);
						break;
					case 2:
						g->z = a_star(g->actuel, g->positionJetons[jeton], c, h_manhattan, g->VerticalWalls, g->DiagupWalls, g->DiagDownWalls);
						break;	
					case 3:
						g->z = a_star(g->actuel, g->positionJetons[jeton], c, h_rapide, g->VerticalWalls, g->DiagupWalls, g->DiagDownWalls);
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
    }
	printf("Fin\n");
	return 0;
}

int recuperer (GameStruct* g, char* file) {
	/* prendre les données et les ajouter à un fichier .txt */
	
	FILE* flux = fopen(file, "a"); // on passe en 1er argument le nom du fichier
	assert(flux != NULL);

	printf("Entrer le nombre de plateau à tester\n");

	int n;
	while(scanf("%d", &n) != 1) {
		while(getchar());
	}


	if (comparaison(g, n, flux) != 0) return -1;

	

	fclose(flux);

	printf(ANSI_COLOR_GREEN "Données Récupérées\n" ANSI_COLOR_RESET);

	return 0;
}
