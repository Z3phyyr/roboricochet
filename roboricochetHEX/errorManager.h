#ifndef ERROR_MANAGER
#define ERROR_MANAGER

#include "../include/SDL.h"
#include "../include/SDL_TTF.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "graphe.h"
#include "struct.h"
#include "textureManager.h"
#include "boardManager.h"
#include "interface.h"

// typedef struct TextureList {
// 	dimTexture* elem;
// 	struct TextureList* next;
// } TList;

// TList* TListAdd (TList* list, dimTexture* e);
// TList* TListRemove (TList* list, dimTexture* e);
// void FreeTList (TList* list);


void SDL_ExitWithError (const char* msg);
/**
 * Exit the program with the error contained in msg.
 * Also displays the latest SDL_Error loaded with SDL_GetError ()
 * \param message to display as error
**/
void DestroyEverything (GameStruct* g);
void throwwithCondition (bool condition, const char* msg, GameStruct* g);

#endif //ERROR_MANAGER