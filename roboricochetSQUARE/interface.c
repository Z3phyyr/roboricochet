#include <math.h>
#include <stdio.h>
#include <string.h>
#include "interface.h"


const int x_heuristiques = 750;
const int y_heuristiques = 500;
const int x_jetons = 1150;
const int y_jetons = 500;


static void SetColor (SDL_Renderer* renderer, SDL_Color color) {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}


int SetButtonTextTexture(SDL_Renderer* renderer, Button* button, const char* texte, 
			int size, const char* font_path, SDL_Color color) {
	TTF_Font* font = TTF_OpenFont(font_path, size);
	SDL_Surface* s = TTF_RenderText_Solid(font, texte, color);
	TTF_CloseFont(font);

	if (s == NULL) return -1;

	button->shape.h = s->h;
	button->shape.w = s->w;

	button->texture = SDL_CreateTextureFromSurface(renderer, s);
	SDL_FreeSurface(s);

	if (button->texture == NULL) return -1;
	
	
	return 0;
}

int SetButtonTexture(SDL_Renderer* renderer, Button* button, dimTexture* atlas, 
			int sub_i, int sub_j, int size) {

	SDL_Texture* t = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, size, size);
	if (t == NULL) return -1;

	button->shape.h = size;
	button->shape.w = size;

	SDL_SetRenderTarget(renderer, t);
	SetColor(renderer, white);
	SDL_RenderClear(renderer);
	if (PrintSubTexture(renderer, atlas, 0, 0, sub_i, sub_j, size) != 0) return -1;
	SDL_SetRenderTarget(renderer, NULL);

	button->texture = t;

	return 0;
}

int AfficherBouton(SDL_Renderer* renderer, Button* b) {
	SetColor(renderer, b->color);
	if (SDL_RenderFillRect(renderer, &b->shape) < 0) {
		return -1;
	}
	if (SDL_RenderCopy(renderer, b->texture, NULL, &b->shape) != 0) {
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
	SDL_Rect effacement = {750, 170, 400, 50};
	if (SDL_RenderFillRect(renderer, &effacement) != 0) return -1;

	if (PrintWrappedText(renderer, line2, FixedSys, black, 30, 750, 170, 400) != 0) {
		return -1;
	}
	return 0;
}



/************************CHOIX HEURISTIQUES*******************/

Button* InitBoutonsHeuristiques(SDL_Renderer* renderer) {
	Button* b = malloc(NB_HEURISTIQUES * sizeof(Button));
	if (b == NULL) return NULL;

	char* textes[] = {
		"Dijkstra",
		"ASTAR: euclidian distance",
		"ASTAR: manhattan distance",
		"ASTAR: manhattan sum distance"
	};
	SDL_Rect r = {x_heuristiques, y_heuristiques, -1, -1};
	for(int k=0; k<NB_HEURISTIQUES; k++) {
		b[k] = (Button) {r, black, darkGrey, NULL};
		if (SetButtonTextTexture(renderer, &b[k], textes[k], 20, consola, magenta) != 0) return NULL;
		r.y += 30;
	}

	return b;
}

void FreeBoutonsHeuristiques(Button* b) {
	if (b != NULL) {
		for(int k=0; k<NB_HEURISTIQUES; k++) {
			SDL_DestroyTexture(b[k].texture);
		}
		free(b);
	}
}


int AfficherChoixHeuristiques (SDL_Renderer* renderer, Button* heuristiques) {
	for(int k=0; k<NB_HEURISTIQUES; k++) {
		if (AfficherBouton(renderer, &heuristiques[k]) != 0) return -1;
	}
	return 0;
}

int AffichageHeuristiqueChoisie(SDL_Renderer* renderer, int choixH) {
	char line1[100] = "Heuristique :";
	char text[100];
	switch (choixH) {
		case 0: 
			sprintf(text, "%s", " Dijkstra");
			break;
		case 1:
			sprintf(text, "%s", " Euclidienne");
			break;
		
		case 2:
			sprintf(text, "%s", " Manhattan");
			break;
		case 3:
			sprintf(text, "%s",  " Manhattan sum");
			break;

		default:
			sprintf(text, "%s", " Aleatoire");
			break;
	}

	strcat(line1, text);
	SetColor(renderer, white);
	if (SDL_RenderFillRect(renderer, &(SDL_Rect) {750, 300, 450, 20}) != 0) return -1;
	if (PrintText(renderer, line1, FixedSys, black, 20, 750, 300) != 0) return -1;

	return 0;
}


/*************************CHOIX JETONS************************/

Button* InitBoutonsJetons(SDL_Renderer* renderer, dimTexture* jetons) {
	Button* b = malloc((NB_JETONS+1) * sizeof(Button));
	if (b == NULL) return NULL;

	SDL_Rect r = {x_jetons, y_jetons, -1, -1};
	for(int k=0; k<NB_JETONS; k++) {
		b[k] = (Button) {r, black, white, NULL};
		if (SetButtonTexture(renderer, &b[k], jetons, k%4, k/4, 40) != 0) return NULL;
		if (k%4 == 3) {
			r.x = x_jetons;
			r.y += 50;
		} else {
			r.x +=50;
		}
	}
	b[NB_JETONS] = (Button) {r, black, white, NULL};
	if (SetButtonTextTexture(renderer, &b[NB_JETONS], "A", 40, consola, black) != 0) return NULL;
	

	return b;
}

void FreeBoutonsJetons(Button* b) {
	if (b != NULL) {
		for(int k=0; k<NB_JETONS+1; k++) {
			SDL_DestroyTexture(b[k].texture);
		}
		free(b);
	}
}

int AfficherChoixJetons(SDL_Renderer* renderer, Button* jetons) {
	for (int k=0; k<NB_JETONS+1; k++) {
		if (AfficherBouton(renderer, &jetons[k]) != 0) return -1;
	}
	return 0;
}

int AffichageJetonChoisi(SDL_Renderer* renderer, int choixJ) {
	char line1[100] = "Jeton :";
	char text[100];
	if (inBounds(choixJ, 0, NB_JETONS-1)) {
		sprintf(text, " %d", choixJ);
	} else {
		sprintf(text, "%s", " Aleatoire");
	}
	
	strcat(line1, text);
	SetColor(renderer, white);
	if (SDL_RenderFillRect(renderer, &(SDL_Rect) {750, 330, 600, 20}) != 0) return -1;
	if (PrintText(renderer, line1, FixedSys, black, 20, 750, 330) != 0) return -1;

	return 0;
}