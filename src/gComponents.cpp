#include "gComponents.h"

#pragma region SpriteComponent

SpriteComponent::SpriteComponent(std::string newPath, Object *parent)
{
	path = newPath;
	texture = NULL;
	renderBox = NULL;
	dim = new iVect;
	gRenderer = nullptr;
	renderBox = new SDL_Rect;
	scale = 1;
	if (parent != nullptr)
	{
		parent->addComponent(this);
	}
}
SpriteComponent::~SpriteComponent()
{
	// this->destroy();
}
void SpriteComponent::whenLinked()
{
	// LOG_INIT_CERR();
	gRenderer = parent->getScene()->getRenderer();
	if (!load(path))
	{
		getParent()->removeComponent(this);
		log(LOG_WARN) << "Removing faulty component (" << this << ")\n";
	}
	log(LOG_INFO) << "Sprite component (" << this << ") linked to " << parent->getName() << "\n";
}
bool SpriteComponent::load(std::string path)
{
	// LOG_INIT_CERR();
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
	// LOG_INIT_CERR();
	if (gRenderer == nullptr)
	{
		std::cout << "gRenderer is nullptr for " << parent->getName() << " in " << parent->getScene()->getName() << "\n";
		return false;
	}
	*renderBox = {(int)parent->getPos().x + offset.x, (int)parent->getPos().y + offset.y, (int)((float)dim->x * scale), (int)((float)dim->y * scale)};
	if (SDL_RenderCopy(gRenderer, texture, NULL, renderBox))
	{
		std::cout << "Texture failed to render for " << parent->getName() << " in " << parent->getScene()->getName() << "\n";
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

RigidBodyComponent::RigidBodyComponent(double newMass, Object *parent)
{
	mass = newMass;
	velocity = {0.0, 0.0};
	force = {0.0, 0.0};
	energyLoss = 1.0;
	hasCollision = false;
	isTrigger = false;
	if (parent != nullptr)
	{
		parent->addComponent(this);
	}
}
RigidBodyComponent::~RigidBodyComponent()
{
	// this->destroy();
}
void RigidBodyComponent::whenLinked()
{
	// LOG_INIT_CERR();
	renderer = parent->getScene()->getRenderer();
	// log(LOG_INFO) << "RigidBody component (" << this << ") linked to " << parent->getName() << "\n";
}
void RigidBodyComponent::applyForce(Vect newForce)
{
	force = newForce;
}
bool RigidBodyComponent::render()
{
	if (drawHitbox && hasCollision)
	{
		SDL_Rect hitBoxVisual{(int)parent->getPos().x + hitBox[0].x, (int)parent->getPos().y + hitBox[0].y, hitBox[1].x - hitBox[0].x, hitBox[1].y - hitBox[0].y};
		SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
		SDL_RenderDrawRect(renderer, &hitBoxVisual);
	}
	return true;
}
bool RigidBodyComponent::update()
{
	collisionList.clear();
	if (mass == 0.0)
	{
		parent->move(parent->getPos() + force * deltaTime);
		return true;
	}
	Vect acceleration = {force.x / mass, force.y / mass};
	velocity += acceleration * deltaTime;
	velocity *= std::pow(1 - energyLoss, deltaTime);
	parent->move(parent->getPos() + velocity * deltaTime);
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
	shootOffset = newPos;
	poolsize = 0;
	cooldownTimer = 0.0;
	bulletLifeSpan = setBulletLifeSpan;
}
template <typename bulletType>
void SpawnerComponent<bulletType>::whenLinked()
{
	// LOG_INIT_CERR();
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
		// Create new bullet
		genericBullet *projectile = new bulletType(parent->getScene(), parent->getPos() + shootOffset);
		projectile->isActive = true;
		pool.push_back(projectile);
		poolsize++;
	}
	else
	{
		// Reuse latest bullet
		pool[0]->move(parent->getPos());
		pool[0]->isActive = true;
		genericBullet *temp = pool[0];
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
	for (genericBullet *bullet : pool)
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

TextComponent::TextComponent(std::string setMessage, Vect setPos, std::string fontPath, Object *parent)
{
	font = nullptr;
	pos = setPos;
	path = setMessage;
	texture = NULL;
	gRenderer = nullptr;
	scale = 1;
	if (fontPath != "")
	{
		setFont(fontPath);
	}
	if (parent != nullptr)
	{
		parent->addComponent(this);
	}
}

bool TextComponent::load(std::string newMessage, SDL_Color newColor, std::string fontPath)
{
	// LOG_INIT_CERR();
	color = newColor;
	if (fontPath != "")
	{
		setFont(fontPath);
	}
	if (font == NULL)
	{
		log(LOG_WARN) << "Text component failed to load font\n";
		return false;
	}
	SDL_Surface *textSurface = TTF_RenderText_Solid(font, path.c_str(), newColor);
	if (textSurface == NULL)
	{
		log(LOG_WARN) << "Text surface failed to render in TextComponent (" << this << ") " << TTF_GetError() << "\n";
		return false;
	}
	texture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
	if (texture == NULL)
	{
		log(LOG_WARN) << "Text surface failed to create texture in TextComponent (" << this << ") " << TTF_GetError() << "\n";
		return false;
	}
	*dim = {textSurface->w, textSurface->h};
	SDL_FreeSurface(textSurface);
	return true;
}

void TextComponent::whenLinked()
{
	// LOG_INIT_CERR();
	gRenderer = parent->getScene()->getRenderer();
	if (!load(path, color))
	{
		getParent()->removeComponent(this);
		log(LOG_WARN) << "Removing faulty component (" << this << ")\n";
	}
	log(LOG_INFO) << "Text component (" << this << ") linked to " << parent->getName() << "\n";
}

void TextComponent::setFont(std::string fontPath, int fontSize)
{
	// LOG_INIT_CERR();
	font = TTF_OpenFont(fontPath.c_str(), fontSize);
	if (font == NULL)
	{
		log(LOG_WARN) << "Font file " << fontPath << " failed to load " << TTF_GetError() << "\n";
	}
}

bool TextComponent::update()
{
	// For now updating text every frame
	// TODO: Make system based on calls
	// load(path);
}

#pragma endregion

#pragma region LayoutHelper

LayoutHelperComponent::LayoutHelperComponent(Layout *setLayout, int setId)
{
	layout = setLayout;
	id = setId;
}
LayoutHelperComponent::~LayoutHelperComponent()
{
	layout->removeObj(id, false);
	layout = nullptr;
}

void LayoutHelperComponent::whenLinked()
{
	// LOG_INIT_CERR();
	log(LOG_INFO) << "LayoutHelper component (" << this << ") linked to " << parent->getName() << "\n";
}

#pragma endregion