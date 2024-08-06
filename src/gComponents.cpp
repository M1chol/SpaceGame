#include "gComponents.h"

#pragma region SpriteComponent

SpriteComponent::SpriteComponent(std::string newPath)
{
	path = newPath;
	texture = NULL;
	renderBox = NULL;
	dim = new iVect;
	gRenderer = nullptr;
	renderBox = new SDL_Rect;
	scale = 1;
}
SpriteComponent::~SpriteComponent()
{
	// this->destroy();
}
void SpriteComponent::whenLinked()
{
	LOG_INIT_CERR();
	gRenderer = parent->getScene()->getRenderer();
	load(path);
	log(LOG_INFO) << "Sprite component (" << this << ") linked to " << parent->getName() << "\n";
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
bool SpriteComponent::render(iVect offset, float newScale)
{
	if (newScale > 0)
	{
		scale = newScale;
	}
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
bool SpriteComponent::render(float newScale)
{
	if (newScale > 0)
	{
		scale = newScale;
	}
	iVect center = {-(int)((float)dim->x * scale / 2), -(int)((float)dim->y * scale / 2)};
	bool status = render(center, scale);
	return status;
}
bool SpriteComponent::render()
{
	return render(0);
}
void SpriteComponent::setScale(float newScale)
{
	if (newScale > 0)
	{
		scale = newScale;
	}
}

#pragma endregion

#pragma region RigidBodyComponent

RigidBodyComponent::RigidBodyComponent(double newMass)
{
	mass = newMass;
	velocity = {0.0, 0.0};
	force = {0.0, 0.0};
	energyLoss = 1.0;
	hasCollision = false;
	isTrigger = false;
}
RigidBodyComponent::~RigidBodyComponent()
{
	// this->destroy();
}
void RigidBodyComponent::whenLinked()
{
	LOG_INIT_CERR();
	log(LOG_INFO) << "RigidBody (" << this << ") linked to " << parent->getName() << "\n";
}
void RigidBodyComponent::applyForce(Vect newForce)
{
	force = newForce;
}
bool RigidBodyComponent::render()
{
	if (drawHitbox && hasCollision)
	{
		SDL_Renderer *renderer = parent->getScene()->getRenderer();
		SDL_Rect hitBoxVisual{(int)parent->pos.x + hitBox[0].x, (int)parent->pos.y + hitBox[0].y, hitBox[1].x - hitBox[0].x, hitBox[1].y - hitBox[0].y};
		SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
		SDL_RenderDrawRect(renderer, &hitBoxVisual);
	}
	return true;
}
bool RigidBodyComponent::update()
{
	collisionList.clear();
	// TODO: add check if speed is beeing changed, then disable energy loss to let objects move at real speed
	if (mass == 0.0)
	{
		parent->pos = parent->pos + force * deltaTime;
		return true;
	}
	Vect acceleration = {force.x / mass, force.y / mass};
	velocity += acceleration * deltaTime;
	velocity *= energyLoss;
	parent->pos += velocity;
	// momentum = velocity * mass;
	return true;
}
void RigidBodyComponent::setCollision(std::vector<iVect> *newHitBox, bool newisTrigger)
{
	hasCollision = true;
	isTrigger = newisTrigger;
	hitBox = *newHitBox;
}
void RigidBodyComponent::setMass(double newMass)
{
	mass = newMass;
}
void RigidBodyComponent::setEnergyLoss(double newEnergyLoss)
{
	energyLoss = 1 - newEnergyLoss;
}
std::vector<iVect> &RigidBodyComponent::getHitBox()
{
	return hitBox;
}
RigidBodyComponent *RigidBodyComponent::isColliding(RigidBodyComponent *obj)
{
	auto el = std::find(collisionList.begin(), collisionList.end(), obj);
	if (el != collisionList.end())
	{
		return *el;
	}
	return nullptr;
}
RigidBodyComponent *RigidBodyComponent::isColliding(TAG tag)
{
	for (RigidBodyComponent *obj : collisionList)
	{
		for (TAG stag : obj->parent->linkedTags)
		{
			if (stag == tag)
			{
				return obj;
			}
		}
	}
	return nullptr;
}
void RigidBodyComponent::solveCollision(RigidBodyComponent *obj)
{
	collisionList.push_back(obj);
	if (!isTrigger)
	{
		// TODO: handle collision
	}
}

#pragma endregion

#pragma region SpawnerComponent
template class SpawnerComponent<genericBullet>;

template <typename bulletType>
SpawnerComponent<bulletType>::SpawnerComponent(Vect newPos, double setCooldown, double setBulletLifeSpan)
{
	cooldown = setCooldown;
	newPos = shootOffset;
	poolsize = 0;
	cooldownTimer = 0.0;
	bulletLifeSpan = setBulletLifeSpan;
}
template <typename bulletType>
void SpawnerComponent<bulletType>::whenLinked()
{
	LOG_INIT_CERR();
	log(LOG_INFO) << "Spawner component (" << this << ") linked to " << parent->getName() << "\n";
}
template <typename bulletType>
void SpawnerComponent<bulletType>::setCooldown(double newCooldown)
{
	cooldown = newCooldown;
}
template <typename bulletType>
bool SpawnerComponent<bulletType>::shoot()
{
	if (cooldownTimer < cooldown)
	{
		return false;
	}
	if (poolsize < 1 || pool[0]->isActive)
	{
		std::shared_ptr<bulletType> projectile = std::make_shared<bulletType>(parent->getScene(), parent->pos + shootOffset);
		projectile->isActive = true;
		pool.push_back(projectile);
		poolsize++;
	}
	else
	{
		pool[0]->pos = parent->pos;
		pool[0]->isActive = true;
		std::shared_ptr<bulletType> temp = pool[0];
		pool.erase(pool.begin());
		pool.push_back(temp);
	}
	cooldownTimer = 0.0;
	return true;
}
template <typename bulletType>
bool SpawnerComponent<bulletType>::update()
{
	cooldownTimer += deltaTime;
	for (std::shared_ptr<genericBullet> bullet : pool)
	{
		if (bullet->aliveFor > bulletLifeSpan)
		{
			bullet->isActive = false;
			bullet->aliveFor = 0.0;
		}
	}
	return true;
}
#pragma endregion

#pragma region TextComponent


#pragma endregion