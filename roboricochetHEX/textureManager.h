#ifndef TEXTUREMANAGER
#define TEXTUREMANAGER
#include "../include/SDL.h"
#include "../include/SDL_ttf.h"

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "struct.h"




bool inBounds(int x, int min, int max);


void DestroyDimTexture(dimTexture* t);

dimTexture* CreateTransparentDimTexture (SDL_Renderer* renderer, 
			int x, int y, int w, int h, int layer, int sub_w, int sub_h);

dimTexture* CreateDimTextureFromTexture (SDL_Renderer* renderer, SDL_Texture* texture, 
			int x, int y, int layer, int sub_w, int sub_h);

dimTexture* CreateDimTextureFromImage (SDL_Renderer* renderer, const char* file, 
			int layer, int sub_w, int sub_h);

dimTexture* CreateDimTextureFromText (SDL_Renderer* renderer, const char* text,
			TTF_Font* font, SDL_Color color, int layer, int sub_w, int sub_h);

/*************PRINTING**************/

int SimpleTexturePrint (SDL_Renderer* renderer, dimTexture* texture);
int PrintSubTexture (SDL_Renderer* renderer, dimTexture* atlas, int x, int y, int num_x, int num_y, int dest_size);
int PrintText (SDL_Renderer* renderer, const char* text, const char* fontPath, const SDL_Color color, const int fontSize, const int x, const int y);
int PrintWrappedText(SDL_Renderer* renderer, const char* text, const char* fontPath, 
			const SDL_Color color, const int fontSize, const int x, const int y, const Uint32 wrapSize);

#endif //TEXTUREMANAGER
