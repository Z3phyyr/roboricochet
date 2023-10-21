#include "errorManager.h"
#include <stdio.h>



void SDL_ExitWithError (const char* msg) {
	SDL_Log("ERREUR : %s > %s\n", msg, SDL_GetError());
	TTF_Quit();
	SDL_Quit();
	exit (1);
}


// TList* TListAdd (TList* list, dimTexture* e) {
// 	//Adds an element to the Texture list while preserving the increasing nature of the layers
// 	if (list == NULL) {
// 		//Empty list
// 		TList* t = malloc(sizeof(TList));
// 		t->elem = e;
// 		t->next = NULL;
// 		return t;
// 	}
// 	dimTexture* x = list->elem;
// 	if (x->layer >= e->layer) {
// 		//The element e is to be placed before x
// 		TList* t = malloc(sizeof(TList));
// 		t->elem = e;
// 		t->next = list;
// 		return t;
// 	}
// 	list->next = TListAdd(list->next, e);
// 	return list;
// }

// TList* TListRemove (TList* list, dimTexture* e) {
// 	//Removes e in the TList list
// 	if (list == NULL) {
// 		return list;
// 	}
// 	if (list->elem == e) {
// 		TList* next = list->next;
// 		free(list);
// 		return next;
// 	}
// 	list->next = TListRemove(list->next, e);
// 	return list;
// }

// void FreeTList (TList* list) {
// 	if (list == NULL) return;

// 	TList* n = list->next;
// 	DestroyDimTexture (list->elem);
// 	free (list);
// 	FreeTList(n);
// }


void DestroyEverything (GameStruct* g) {
	DestroyDimTexture(g->jetons);
	DestroyDimTexture(g->robots);
	SDL_DestroyRenderer(g->renderer);
	SDL_DestroyWindow(g->window);
}

void throwwithCondition (bool condition, const char* msg, GameStruct* g) {
	if (condition) {
		DestroyEverything(g);
		SDL_ExitWithError(msg);
	}
}