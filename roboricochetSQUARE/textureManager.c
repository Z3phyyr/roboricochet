#include "textureManager.h"


bool inBounds(int x, int min, int max) {
	return x>=min && x<=max;
}

void DestroyDimTexture(dimTexture* t) {
	if (t == NULL) {
		return;
	}
	SDL_DestroyTexture(t->texture);
	free(t);
}


SDL_Texture* TextureFromImageCreator (SDL_Renderer* renderer, const char* file, int* w, int* h) {
	//Creates an standard SDL_Texture from a image
	SDL_Surface* image = NULL;
	SDL_Texture* texture = NULL; 
	image = SDL_LoadBMP(file);

	if (image == NULL) {
		return NULL;
	}
	if (h != NULL) *h = image->h;
	if (w != NULL) *w = image->w;

	texture = SDL_CreateTextureFromSurface(renderer, image);
	SDL_FreeSurface(image);

	return texture;
}

SDL_Texture* TextureFromTextCreator (SDL_Renderer* renderer, const char* text, 
				TTF_Font* font, const SDL_Color color, int* w, int* h) {
	//Creates an standard SDL_Texture from a text
	SDL_Surface* text_surface = NULL;
	SDL_Texture* texture = NULL;

	text_surface = TTF_RenderText_Solid (font, text, color);

	if (text_surface == NULL) {
		return NULL;
	}
	if (h != NULL) *h = text_surface->h;
	if (w != NULL) *w = text_surface->w;

	texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	SDL_FreeSurface(text_surface);

	return texture;
}

SDL_Texture* TransparentTexture (SDL_Renderer* renderer, int w, int h) {
	//Creates a transparent invisible SDL_Texture 
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, w, h);
	if (texture == NULL || SDL_SetRenderTarget(renderer, texture) != 0) {
		SDL_DestroyTexture(texture);
		return NULL;
	}
	

	int check1 = SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	//int check2 = SDL_SetTextureAlphaMod(texture, 0);
	if (check1  /*check2*/) {
		SDL_DestroyTexture(texture);
		return NULL;
	}

	SDL_SetRenderTarget(renderer, NULL);

	return texture;
}

dimTexture* CreateTransparentDimTexture (SDL_Renderer* renderer, int x, int y, int w, int h, int layer, int sub_w, int sub_h) {
	//Creates a Transparent dimTexture
	dimTexture* dt = malloc(sizeof(dimTexture));
	assert(dt != NULL);
	dt->texture = TransparentTexture(renderer, w, h);
	if (dt->texture == NULL) return NULL;
	dt->dimensions = (SDL_Rect){x, y, w, h};
	dt->layer = layer;
	dt->sub_w = sub_w; dt->sub_h = sub_h;
	return dt;
}

dimTexture* CreateDimTextureFromTexture (SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, int layer, int sub_w, int sub_h) {
	//Creates a dimTexture from an already existing texture
	dimTexture* dt = malloc(sizeof(dimTexture));
	assert(dt != NULL);
	dt->texture = texture;
	int w, h;
	if (SDL_QueryTexture(texture, NULL, NULL, &w, &h) != 0) return NULL;
	dt->dimensions = (SDL_Rect){x, y, w, h};
	dt->layer = layer;
	dt->sub_w = sub_w; dt->sub_h = sub_h;
	return dt;
}

dimTexture* CreateDimTextureFromImage (SDL_Renderer* renderer, const char* file, int layer, int sub_w, int sub_h) {
	//Creates a dimTexture from an Image
	dimTexture* dt = malloc(sizeof(dimTexture));
	assert(dt != NULL);
	SDL_Texture* texture = TextureFromImageCreator(renderer, file, &dt->dimensions.w, &dt->dimensions.h);
	if (texture == NULL) {
		free(dt);
		return NULL;
	}

	
	dt->texture = texture;
	dt->dimensions.x = 0; dt->dimensions.y = 0;
	dt->layer = layer;
	dt->sub_w = sub_w; dt->sub_h = sub_h;
	return dt;
}

dimTexture* CreateDimTextureFromText (SDL_Renderer* renderer, const char* text,
			TTF_Font* font, SDL_Color color, int layer, int sub_w, int sub_h) {
	//Creates a dimTexture from text
	dimTexture* dt = malloc(sizeof(dimTexture));	
	assert(dt != NULL);
	dt->texture = TextureFromTextCreator(renderer, text, font, color, &dt->dimensions.w, &dt->dimensions.h);
	dt->dimensions.x = 0; dt->dimensions.y = 0;
	dt->layer = layer;
	dt->sub_w = sub_w; dt->sub_h = sub_h;
	return dt;
}


/**************************************************************************/

int SimpleTexturePrint (SDL_Renderer* renderer, dimTexture* texture) {
	//Prints the texture held in a dimTexture
	if (texture == NULL) return -1;
	if (SDL_RenderCopy(renderer, texture->texture, NULL, &texture->dimensions) != 0) {
		return -1;
	}
	return 0;
}


int PrintSubTexture (SDL_Renderer* renderer, dimTexture* atlas, int x, int y, int num_x, int num_y, int dest_size) {
	if (atlas == NULL) return -2;

	SDL_Rect SubTextureDim = {num_x * atlas->sub_w, num_y * atlas->sub_h, atlas->sub_w, atlas->sub_h};

	SDL_Rect dstRect = {x, y, dest_size, dest_size};

	return SDL_RenderCopy(renderer, atlas->texture, &SubTextureDim, &dstRect);
}




int PrintText (SDL_Renderer* renderer, const char* text, char* fontPath, const SDL_Color color, const int fontSize, const int x, const int y) {
	TTF_Font* font = TTF_OpenFont(fontPath, fontSize);

	if (font == NULL) return -1;
	
	SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
	TTF_CloseFont(font);

	if (surface == NULL) return -1;

	int w = surface->w, h = surface->h;

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	if (texture == NULL) return -1;

	SDL_Rect destrect = {x, y, w, h};
	
	SDL_RenderCopy(renderer, texture, NULL, &destrect);


	SDL_DestroyTexture(texture);

	return 0;
}