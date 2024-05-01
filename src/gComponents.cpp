#include "gComponents.h"

SpriteComponent::SpriteComponent(SDL_Renderer* renderer, string path=""){
	LOG_INIT_CERR();
    gRenderer = renderer;
	texture = NULL;
	renderBox = NULL;
	dim = new iVect;
	renderBox = new SDL_Rect;
    if(path != ""){
        load(path);
    }
	log(LOG_INFO) << "SpriteComponent created\n";
}

SpriteComponent::~SpriteComponent(){
    free();
}

bool SpriteComponent::load(string path){
    LOG_INIT_CERR();
	SDL_Texture* new_texture = NULL;
	SDL_Surface* loaded = IMG_Load(path.c_str());
	if(loaded == NULL){
		log(LOG_WARN) << "Failed loading media " << IMG_GetError() << "\n";
		return false;
	}
	new_texture = SDL_CreateTextureFromSurface(gRenderer, loaded);
	if(new_texture == NULL){
		log(LOG_WARN) << "Failed converting media to texture " << IMG_GetError() << "\n";
		return false;
	}
	*dim = {loaded->w, loaded->h};
	SDL_FreeSurface(loaded);
    texture = new_texture;
	return true;
}

iVect* SpriteComponent::getDim(){
	return dim;
}
// Renders image at offset with set scale
void SpriteComponent::render(iVect offset, float scale){
	*renderBox = {(int)parent->pos.x + offset.x, (int)parent->pos.y + offset.y, (int)((float)dim->x*scale), (int)((float)dim->y*scale)};
	SDL_RenderCopy(gRenderer, texture, NULL, renderBox);
}
// Renders centered image at scale
void SpriteComponent::render(float scale){
	iVect center = {-(int)((float)dim->x*scale/2), -(int)((float)dim->y*scale/2)};
	render(center, scale);
}
// Renders centered image
void SpriteComponent::render(){
	render(1);
}

void SpriteComponent::free(){
    SDL_DestroyTexture(texture);
}