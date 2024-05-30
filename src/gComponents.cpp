#include "gComponents.h"

#pragma region SpriteComponent definitions

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
	std::cout << "Do not usem delete on SpriteComponent instad use comp->destoy()";
}
void SpriteComponent::whenLinked()
{
	gRenderer = parent->getScene()->getRenderer();
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
		log(LOG_WARN) << "gRenderer is nullptr for " << parent->getName() << " in " << parent->getScene()->getName() << "\n";
		return false;
	}
	*renderBox = {(int)parent->pos.x + offset.x, (int)parent->pos.y + offset.y, (int)((float)dim->x * scale), (int)((float)dim->y * scale)};
	if (SDL_RenderCopy(gRenderer, texture, NULL, renderBox))
	{
		log(LOG_WARN) << "Texture failed to render for " << parent->getName() << " in " << parent->getScene()->getName() << "\n";
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
	LOG_INIT_CERR();
	SDL_DestroyTexture(texture);
	if (!parent->removeComponent(this))
	{
		log(LOG_WARN) << "Could not remove component from object\n";
	}
}

#pragma endregion

#pragma region RigidBodyComponent definitions

RigidBodyComponent::RigidBodyComponent(double newMass)
{
	mass = newMass;
	velocity = {0.0, 0.0};
	force = {0.0, 0.0};
	energyLoss = 1.0;
}
RigidBodyComponent::~RigidBodyComponent() {}
void RigidBodyComponent::applyForce(Vect newForce)
{
	force = newForce;
}
bool RigidBodyComponent::render()
{
	if (drawHitbox)
	{
		// TODO: Implement drawing of hitbox using SDL shapes
	}
	return true;
}
bool RigidBodyComponent::update()
{
	// TODO: Implement case when mass != 0
	if (mass == 0.0)
	{
		parent->pos = parent->pos + force * deltaTime;
		return true;
	}
	Vect acceleration = {force.x / mass, force.y / mass};
	velocity += acceleration * deltaTime;
	velocity *= energyLoss;
	parent->pos += velocity;
}
void RigidBodyComponent::setMass(double newMass)
{
	mass = newMass;
}
void RigidBodyComponent::setEnergyLoss(double newEnergyLoss)
{
	energyLoss = newEnergyLoss;
}

#pragma endregion