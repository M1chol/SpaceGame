#include "gComponents.h"

#pragma region SpriteComponent

SpriteComponent::SpriteComponent(std::string newPath, Object *parent)
{
	name = "SpriteComponent";
	path = newPath;
	texture = nullptr;
	renderBox = nullptr;
	gRenderer = nullptr;
	offset = {0, 0};
	center = {0, 0};
	dim = {0, 0};
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
	SDL_DestroyTexture(texture);
	delete renderBox;
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
	dim = {loaded->w, loaded->h};
	calculateCenter();

	SDL_FreeSurface(loaded);
	texture = new_texture;
	return true;
}
iVect SpriteComponent::getDim()
{
	return dim;
}
bool SpriteComponent::render(int index, iVect newOffset, float newScale)
{
	if (gRenderer == nullptr)
	{
		log(LOG_WARN) << "gRenderer is nullptr for " << parent->getName() << " in " << parent->getScene()->getName() << "\n";
		return false;
	}
	sheetIndex = index;

	offset = newOffset;
	if (newScale > 0)
	{
		scale = newScale;
	}
	iVect imgSize = dim;
	if (index > -1)
	{
		imgSize = {sheetSize, sheetSize};
	}

	*renderBox = {(int)parent->getPos().x + offset.x,
				  (int)parent->getPos().y + offset.y,
				  (int)((float)imgSize.x * scale),
				  (int)((float)imgSize.y * scale)};

	SDL_Rect sheet;
	if (index > -1)
	{
		int spriteSheetCol = (sheetIndex % (dim.x / sheetSize)); // Column of the sprite
		int spriteSheetRow = (sheetIndex / (dim.x / sheetSize)); // Row of the sprite

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
	return render(index, offset, newScale);
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
	calculateCenter();
}

void SpriteComponent::calculateCenter()
{
	center = {-(int)((float)dim.x * scale / 2),
			  -(int)((float)dim.y * scale / 2)};
}

void SpriteComponent::setSheetIndex(int index)
{
	int spriteSheetCol = (sheetIndex % (dim.x / sheetSize));
	int spriteSheetRow = (sheetIndex / (dim.x / sheetSize));

	sheetIndex = index;
	center.x = sheetSize / 2 + sheetSize * spriteSheetCol;
	center.y = -sheetSize / 2 - sheetSize * spriteSheetRow;
	center *= scale;
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
// TODO: Reimplement SpawnerComponent using Object *
#pragma endregion

#pragma region TextComponent

TextComponent::TextComponent(std::string setMessage, iVect newOffset, std::string setfontPath, int setFontSize, Object *setParent)
{
	name = "TextComponent";
	font = nullptr;
	color = {255, 255, 255, 255};
	offset = newOffset;
	path = setMessage;
	texture = NULL;
	gRenderer = nullptr;
	parent = nullptr;
	scale = 1;
	fontSize = setFontSize;
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

bool TextComponent::load(std::string newMessage, SDL_Color newColor, std::string setFontPath, int fontSize)
{
	color = newColor;
	if (newMessage != "")
	{
		path = newMessage;
	}
	if (setFontPath != "")
	{
		fontPath = setFontPath;
		setFont(setFontPath, fontSize);
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
	dim = {textSurface->w, textSurface->h};
	calculateCenter();

	SDL_FreeSurface(textSurface);
	return true;
}

void TextComponent::setColor(SDL_Color newColor)
{
	color = newColor;
	load(path, color, fontPath, fontSize);
}

void TextComponent::setSize(int newSize)
{
	fontSize = newSize;
	load(path, color, fontPath, fontSize);
}

void TextComponent::whenLinked()
{
	if (path == "")
	{
		path = parent->getName();
		load(path);
	}
	gRenderer = parent->getScene()->getRenderer();
	if (!load(path, color, fontPath, fontSize))
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
}

#pragma endregion

#pragma region LayoutHelper
// TODO: Implement GridComponent - arranges children to specefied grid
#pragma endregion