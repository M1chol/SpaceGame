#include "gComponents.h"

#pragma region SpriteComponent

SpriteComponent::SpriteComponent(std::string newPath, Object *parent)
{
	name = "SpriteComponent";
	path = newPath;
	texture = nullptr;
	renderBox = nullptr;
	gRenderer = nullptr;
	offset = nullptr;
	dim = new iVect;
	renderBox = new SDL_Rect;
	scale = 1;
	sheetIndex = -1;
	sheetSize = 100;
	rotation = 0;
	if (parent != nullptr)
	{
		parent->addComponent(this);
	}
}
SpriteComponent::~SpriteComponent()
{
	delete dim;
	delete renderBox;
	delete offset;
	texture = nullptr;
	gRenderer = nullptr;
}
void SpriteComponent::whenLinked()
{

	gRenderer = parent->getScene()->getRenderer();
	if (!load(path))
	{
		getParent()->removeComponent(this);
		log(LOG_WARN) << "Removing faulty component (" << this << ")\n";
	}
	log(LOG_INFO) << "Sprite component (" << this << ") linked to " << parent->getName() << "\n";
}
bool SpriteComponent::load(std::string setPath)
{
	if (setPath != "")
	{
		path = setPath;
	}
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
bool SpriteComponent::render(int index, iVect *newOffset, float newScale)
{
	if (gRenderer == nullptr)
	{
		log(LOG_WARN) << "gRenderer is nullptr for " << parent->getName() << " in " << parent->getScene()->getName() << "\n";
		return false;
	}
	sheetIndex = index;
	if (newOffset != nullptr)
	{
		offset = newOffset;
	}
	if (newScale > 0)
	{
		scale = newScale;
	}
	iVect imgSize = *dim;
	if (index > -1)
	{
		imgSize = {sheetSize, sheetSize};
	}
	if (offset == nullptr && newOffset == nullptr)
	{
		offset = new iVect{-(int)((float)imgSize.x * scale / 2),
						   -(int)((float)imgSize.y * scale / 2)};
	}
	*renderBox = {(int)parent->getPos().x + offset->x,
				  (int)parent->getPos().y + offset->y,
				  (int)((float)imgSize.x * scale),
				  (int)((float)imgSize.y * scale)};

	SDL_Rect sheet;
	if (index > -1)
	{
		int spriteSheetCol = (sheetIndex % (dim->x / sheetSize)); // Column of the sprite
		int spriteSheetRow = (sheetIndex / (dim->x / sheetSize)); // Row of the sprite

		sheet = {spriteSheetCol * sheetSize,
				 spriteSheetRow * sheetSize,
				 sheetSize,
				 sheetSize};
	}

	if (SDL_RenderCopyEx(gRenderer, texture, (index > -1 ? &sheet : NULL), renderBox, rotation + parent->getRotation(), NULL, SDL_FLIP_NONE))
	{
		log(LOG_WARN) << "Texture failed to render for " << parent->getName() << " in " << parent->getScene()->getName() << "\n";
		return false;
	}
	return true;
}
bool SpriteComponent::render(int index, float newScale)
{
	return render(index, nullptr, scale);
}

bool SpriteComponent::render()
{
	return render(sheetIndex, scale);
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
	name = "RigidBodyComponent";
	mass = newMass;
	velocity = {0.0, 0.0};
	force = {0.0, 0.0};
	energyLoss = 1.0;
	hasCollision = false;
	isTrigger = false;
	isClickable = false;
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

	renderer = parent->getScene()->getRenderer();
	log(LOG_INFO) << "RigidBody component (" << this << ") linked to " << parent->getName() << "\n";
}
void RigidBodyComponent::applyForce(Vect newForce)
{
	force = newForce;
}
bool RigidBodyComponent::render()
{
	if (drawDebug && hasCollision)
	{
		iVect parentPos = parent->getPos().toIVect();
		SDL_Rect hitBoxVisual{parentPos.x - hitBox.x, parentPos.y - hitBox.y, hitBox.w, hitBox.h};
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
void RigidBodyComponent::setCollision(SDL_Rect *newHitBox, bool newisTrigger)
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
SDL_Rect *RigidBodyComponent::getHitBox()
{
	return &hitBox;
}

iVect RigidBodyComponent::getHitBox(int index)
{
	iVect result = {0, 0};
	if (index == 0)
	{
		result = {hitBox.x, hitBox.y};
	}
	else if (index == 1)
	{
		result = {hitBox.x - hitBox.w, hitBox.y - hitBox.h};
	}
	else
	{
		log(LOG_WARN) << "RigidBodyComponent::getHitBox ( " << this << " ), not supported index\n";
	}
	return result;
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

// TODO: Remake SpawnerComponent to use Object * instead of template
template <typename bulletType>
SpawnerComponent<bulletType>::SpawnerComponent(Object *parent, Vect newPos, double setCooldown, double setBulletLifeSpan)
{
	name = "SpawnerComponent";
	cooldown = setCooldown;
	shootOffset = newPos;
	poolsize = 0;
	cooldownTimer = cooldown;
	bulletLifeSpan = setBulletLifeSpan;
	if (parent != nullptr)
	{
		parent->addComponent(this);
	}
}
template <typename bulletType>
SpawnerComponent<bulletType>::~SpawnerComponent()
{
	for (bulletType *bullet : pool)
	{
		delete bullet;
	}
}
template <typename bulletType>
void SpawnerComponent<bulletType>::whenLinked()
{
	log(LOG_INFO) << "Spawner component (" << this << ") linked to " << parent->getName() << "\n";
}
template <typename bulletType>
void SpawnerComponent<bulletType>::setCooldown(double newCooldown)
{
	cooldown = newCooldown;
}
template <typename bulletType>
bool SpawnerComponent<bulletType>::shoot(double angle)
{
	if (cooldownTimer < cooldown)
	{
		return false;
	}
	if (poolsize < 1 || pool[0]->isActive)
	{
		// Create new bullet
		genericBullet *projectile = new bulletType(parent->getScene(), parent->getPos() + shootOffset);
		projectile->reset(angle, 1000, parent->getPos() + shootOffset);
		pool.push_back(projectile);
		poolsize++;
	}
	else
	{
		// Reuse latest bullet
		// TODO: can be optimized to not resize vector each frame use lookup table instead
		pool[0]->reset(angle, 1000, parent->getPos() + shootOffset);
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

TextComponent::TextComponent(std::string setMessage, Vect setPos, std::string setfontPath, Object *setParent)
{
	name = "TextComponent";
	font = nullptr;
	pos = setPos;
	path = setMessage;
	texture = NULL;
	gRenderer = nullptr;
	parent = nullptr;
	scale = 1;
	fontPath = "";
	if (setfontPath != "")
	{
		fontPath = setfontPath;
		setFont(setfontPath);
	}
	if (setParent != nullptr)
	{
		parent = setParent;
		parent->addComponent(this);
	}
	if (parent != nullptr)
	{
		gRenderer = parent->getScene()->getRenderer();
	}
}

bool TextComponent::load(std::string newMessage, SDL_Color newColor, std::string setFontPath)
{
	color = newColor;
	if (newMessage != "")
	{
		path = newMessage;
	}
	if (setFontPath != "")
	{
		fontPath = setFontPath;
		setFont(setFontPath);
	}
	if (font == NULL)
	{
		log(LOG_WARN) << "Text component failed to load font\n";
		return false;
	}
	SDL_Surface *textSurface = TTF_RenderUTF8_Blended(font, path.c_str(), newColor);
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

	gRenderer = parent->getScene()->getRenderer();
	if (!load(path, color))
	{
		getParent()->removeComponent(this);
		log(LOG_WARN) << "Removing faulty component (" << this << ")\n";
	}
	log(LOG_INFO) << "Text component (" << this << ") linked to " << parent->getName() << "\n";
}

void TextComponent::setFont(std::string setFontPath, int fontSize)
{

	font = TTF_OpenFont(setFontPath.c_str(), fontSize);
	fontPath = setFontPath;
	if (font == NULL)
	{
		log(LOG_WARN) << "Font file " << setFontPath << " failed to load " << TTF_GetError() << "\n";
	}
}

bool TextComponent::update()
{
	if (path == "")
	{
		load(parent->getName());
		return true;
	}
	return false;
}

#pragma endregion

#pragma region LayoutHelper

LayoutHelperComponent::LayoutHelperComponent(Layout *setLayout, int setId)
{
	name = "LayoutHelperComponent";
	layout = setLayout;
	id = setId;
	if (layout != nullptr)
	{
		layoutID = layout->getID();
	}
}
LayoutHelperComponent::~LayoutHelperComponent()
{
	layout->removeObj(id, false);
}

void LayoutHelperComponent::whenLinked()
{
	log(LOG_INFO) << "LayoutHelper component (" << this << ") linked to " << parent->getName() << "\n";
}

void CustomUpdateComponent::whenLinked()
{
	log(LOG_INFO) << "CustomUpdate component (" << this << ") linked to " << parent->getName() << "\n";
}

CustomUpdateComponent::CustomUpdateComponent(Object *parent)
{
	name = "CustomUpdateComponent";
	if (parent != nullptr)
	{
		parent->addComponent(this);
	}
	updateFunc = nullptr;
}

#pragma endregion