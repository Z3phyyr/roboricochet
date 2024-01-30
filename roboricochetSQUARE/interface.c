#include <math.h>
#include <string.h>
#include "interface.h"


void SetColor (SDL_Renderer* renderer, SDL_Color color) {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

int AfficherBouton(SDL_Renderer* renderer, Button* b) {
	SetColor(renderer, b->color);
	if (SDL_RenderFillRect(renderer, &b->shape) < 0) {
		return -1;
	}
	SetColor(renderer, b->outline_color);
	if (SDL_RenderDrawRect(renderer, &b->shape) < 0) {
		return -1;
	}
	return 0;
}


bool CheckBoutonPresse(Button* b, int x, int y) {
	return (inBounds(x, b->shape.x, b->shape.x + b->shape.w)
			&& inBounds(y, b->shape.y, b->shape.y + b->shape.h));
}

int AffichageTexteInformatif(SDL_Renderer* renderer, int distance_totale) {
	const char* FixedSys = "C:/Program_Files/OCaml64/home/Admin/C/SDL/roboricochetSQUARE/fonts/FixedSys.ttf";
	char line1[140] = "Nombre de coups du chemin = ";
	char line2[115] = "Position 0 / ";
	
	
	
	char dist[100];
	sprintf(dist, "%d", distance_totale);
	strcat(line1, dist);

	strcat(line2, dist);

	//Efface ce qu'il y avait auparavant
	SetColor(renderer, white);
	SDL_Rect effacement = {750, 100, 400, 150};
	if (SDL_RenderFillRect(renderer, &effacement) != 0) return -1;


	if (PrintWrappedText(renderer, line1, FixedSys, 
			black, 30, 750, 100, 400) != 0 ||
		PrintWrappedText(renderer, line2, FixedSys, 
			black, 30, 750, 170, 400) != 0) {
				return -1;
	}
	return 0;
}

int AffichagePosition(SDL_Renderer* renderer, int distance, int distance_totale) {
	const char* FixedSys = "C:/Program_Files/OCaml64/home/Admin/C/SDL/roboricochetSQUARE/fonts/FixedSys.ttf";
	char line2[115] = "Position ";

	char dist_tot[100];
	char dist[100];

	sprintf(dist_tot, "%d", distance_totale);
	sprintf(dist, "%d", distance);

	strcat(line2, dist);
	strcat(line2, " / ");
	strcat(line2, dist_tot);

	//Efface ce qu'il y avait auparavant
	SetColor(renderer, white);
	SDL_Rect effacement = {750, 170, 400, 150};
	if (SDL_RenderFillRect(renderer, &effacement) != 0) return -1;

	if (PrintWrappedText(renderer, line2, FixedSys, black, 30, 750, 170, 400) != 0) {
		return -1;
	}
	return 0;
}