#include "gComponents.h"

SpriteComponent::SpriteComponent(SDL_Renderer* renderer, string path=""){
    gRenderer = renderer;
    if(path != ""){
        load(path);
        printf("TEST");
    }
}

SpriteComponent::~SpriteComponent(){
    free();
}

bool SpriteComponent::load(string path){
    LOG_INIT_CERR();
	SDL_Texture* new_texture = NULL;
	SDL_Surface* loaded = IMG_Load(path.c_str());
	if(loaded == NULL){
		log(LOG_WARN) << "Failed loading media (" << path << ") " << IMG_GetError() << "\n";
		return false;
	}
	new_texture = SDL_CreateTextureFromSurface(gRenderer, loaded);
	if(new_texture == NULL){
		log(LOG_WARN) << "Failed converting media to texture (" << path << ") " << IMG_GetError() << "\n";
		return false;
	}
	SDL_FreeSurface(loaded);
    texture = new_texture;
	return true;
}

void SpriteComponent::simpleRender(iVect pos){
    SDL_RenderCopy(gRenderer, texture, NULL, NULL);
}

void SpriteComponent::free(){
    SDL_DestroyTexture(texture);
}