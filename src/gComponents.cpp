#include "gComponents.h"

SpriteComponent::SpriteComponent(std::string newPath)
{
	LOG_INIT_CERR();
	path = newPath;
	texture = NULL;
	renderBox = NULL;
	dim = new iVect;
	gRenderer = nullptr;
	renderBox = new SDL_Rect;
	log(LOG_INFO) << "SpriteComponent created\n";
}

SpriteComponent::~SpriteComponent()
{
	destroy();
}

void SpriteComponent::whenLinked()
{
	gRenderer = parent->linkedScene->getRenderer();
	load(path);
}

bool SpriteComponent::load(std::string path)
{
	LOG_INIT_CERR();
	SDL_Texture *new_texture = NULL;

	SDL_Surface *loaded = IMG_Load(path.c_str());
	if (loaded == NULL)
	{
		log(LOG_WARN) << "Failed loading media " << IMG_GetError() << "\n";
		return false;
	}
	new_texture = SDL_CreateTextureFromSurface(gRenderer, loaded);
	if (new_texture == NULL)
	{
		log(LOG_WARN) << "Failed converting media to texture " << IMG_GetError() << "\n";
		return false;
	}
	*dim = {loaded->w, loaded->h};
	SDL_FreeSurface(loaded);
	texture = new_texture;
	return true;
}

iVect *SpriteComponent::getDim()
{
	return dim;
}

bool SpriteComponent::render(iVect offset, float scale)
{
	LOG_INIT_CERR();
	if (gRenderer == nullptr)
	{
		// log(LOG_WARN) << "SpriteComponent::render gRenderer is nullptr\n";
		return false;
	}
	*renderBox = {(int)parent->pos.x + offset.x, (int)parent->pos.y + offset.y, (int)((float)dim->x * scale), (int)((float)dim->y * scale)};
	if (SDL_RenderCopy(gRenderer, texture, NULL, renderBox))
	{
		log(LOG_WARN) << "Element failed to render, " << SDL_GetError() << "\n";
		return false;
	}
	return true;
}
bool SpriteComponent::render(float scale)
{
	iVect center = {-(int)((float)dim->x * scale / 2), -(int)((float)dim->y * scale / 2)};
	bool status = render(center, scale);
	return status;
}
bool SpriteComponent::render()
{
	bool status = render(1);
	return status;
}

void SpriteComponent::destroy()
{
	SDL_DestroyTexture(texture);
	parent->removeComponent(this);
}