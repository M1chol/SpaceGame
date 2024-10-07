#include <engine.h>

Component *loadAndAttach(std::ifstream &in, Object *obj);

#pragma region Layouts

void Layout::saveBin(std::ofstream &out)
{
    // ID
    out.write((char *)&ID, sizeof(ID));
    // Object
    Object::saveBin(out);
}
void Layout::loadBin(std::ifstream &in)
{
    // ID
    in.read((char *)&ID, sizeof(ID));
    // Object
    Object::loadBin(in);
}

void Grid::saveBin(std::ofstream &out)
{
    // Layout
    Layout::saveBin(out);
    // Size
    out.write((char *)&size, sizeof(size));
    // Cell size
    out.write((char *)&cellSize, sizeof(cellSize));
    // Linked Objects
    int nrOfObjects = linkedObjects.size();
    out.write((char *)&nrOfObjects, sizeof(nrOfObjects));
    for (int i = 0; i < nrOfObjects; i++)
    {
        Object *obj = linkedObjects[i];
        int objectId = linkedObjectsId[i];
        out.write((char *)&objectId, sizeof(objectId));
        obj->saveBin(out);
    }
}

void Grid::loadBin(std::ifstream &in)
{
    // Layout
    Layout::loadBin(in);
    // Size
    in.read((char *)&size, sizeof(size));
    // Cell size
    in.read((char *)&cellSize, sizeof(cellSize));
    // Calculate center
    calculateGridCenter();
    // Linked Objects
    int nrOfObjects;
    in.read((char *)&nrOfObjects, sizeof(nrOfObjects));
    for (int i = 0; i < nrOfObjects; i++)
    {
        int objectId;
        in.read((char *)&objectId, sizeof(objectId));
        linkedObjectsId.push_back(objectId);
        Object *obj = new Object(getScene());
        obj->loadBin(in);
        linkedObjects.push_back(obj);
    }
    log(LOG_INFO) << "Loaded grid with " << nrOfObjects << " Objects\n";
}
#pragma endregion
#pragma region Object

void Object::saveBin(std::ofstream &out)
{
    // Pos
    out.write((char *)&pos, sizeof(pos));
    // Scene by scene id
    int sceneID = getSceneID(linkedScene);
    out.write((char *)&sceneID, sizeof(sceneID));
    // Name
    int nameLength = name.size();
    out.write((char *)&nameLength, sizeof(nameLength));
    out.write(name.c_str(), nameLength);
    // Components
    out.write((char *)&nrOfComponents, sizeof(nrOfComponents));
    for (auto *component : componentList)
    {
        component->saveBin(out);
    }
    log(LOG_OK) << "Saved " << name << " with " << nrOfComponents << " Components\n";
}
void Object::loadBin(std::ifstream &in)
{
    // Pos
    in.read((char *)&pos, sizeof(pos));
    // Scene by scene id
    int sceneID;
    in.read((char *)&sceneID, sizeof(sceneID));
    linkedScene = sceneList[sceneID];
    // Name
    int nameLength;
    in.read((char *)&nameLength, sizeof(nameLength));
    name.resize(nameLength);
    in.read(&name[0], nameLength);

    // Components
    size_t componentCount;
    in.read((char *)&componentCount, sizeof(componentCount));
    for (int i = 0; i < componentCount; i++)
    {
        loadAndAttach(in, this);
    }
    log(LOG_OK) << "Loaded " << name << " (" << this << ") with " << nrOfComponents << " Components\n";
}

#pragma endregion
#pragma region Component

void Component::saveBin(std::ofstream &out) {}
void Component::loadBin(std::ifstream &in) {}

Component *loadAndAttach(std::ifstream &in, Object *obj)
{
    ComponentType type;
    in.read((char *)&type, sizeof(type));
    Component *comp = nullptr;
    switch (type)
    {
    case COMP_TYPE_SPRITE:
        comp = new SpriteComponent();
        break;
    case COMP_TYPE_RIGIDBODY:
        comp = new RigidBodyComponent();
        break;
    case COMP_TYPE_SPAWNER:

        SpawnerBulletType bType;
        in.read((char *)&bType, sizeof(bType));
        switch (bType)
        {
        case SPAWNER_BULLET_GENERIC:
            comp = new SpawnerComponent<genericBullet>();
            break;
        default:
            log(LOG_WARN) << "Loading SpawnerComponent failed, unknown SpawnerBulletType\n";
            return nullptr;
            break;
        }

        break;

    case COMP_TYPE_TEXT:
        comp = new TextComponent();
        break;

    case COMP_TYPE_LAYOUT:
        comp = new LayoutHelperComponent();
        break;

    default:
        log(LOG_WARN) << "Loading of some Component failed, unknown ComponentType\n";
        return nullptr;
        break;
    }
    comp->setParent(obj);
    comp->loadBin(in);
    obj->addComponent(comp);
    return comp;
}

void SpriteComponent::saveBin(std::ofstream &out)
{
    // Type
    ComponentType type = COMP_TYPE_SPRITE;
    out.write((char *)&type, sizeof(type));
    // Path
    size_t pathlength = path.size();
    out.write((char *)&pathlength, sizeof(pathlength));
    out.write(path.c_str(), pathlength);
    // Scale
    out.write((char *)&scale, sizeof(scale));
}
void SpriteComponent::loadBin(std::ifstream &in)
{
    // Path
    size_t pathLength;
    in.read((char *)&pathLength, sizeof(pathLength));
    path.resize(pathLength);
    in.read(&path[0], pathLength);
    // Scale
    in.read((char *)&scale, sizeof(scale));
    // Load image
    gRenderer = parent->getScene()->getRenderer();
    // log(LOG_INFO) << "SPRITE: pathLength: " << pathLength << ", path: " << path.c_str() << ", scale: " << scale << "\n";
}

void RigidBodyComponent::saveBin(std::ofstream &out)
{
    // Type
    ComponentType type = COMP_TYPE_RIGIDBODY;
    out.write((char *)&type, sizeof(type));
    // Mass
    out.write((char *)&mass, sizeof(mass));
    // Energy loss
    out.write((char *)&energyLoss, sizeof(energyLoss));
    // Has Collision
    out.write((char *)&hasCollision, sizeof(hasCollision));
    // Is trigger
    out.write((char *)&isTrigger, sizeof(isTrigger));
    // hitBox
    out.write((char *)&hitBox, sizeof(hitBox));
}
void RigidBodyComponent::loadBin(std::ifstream &in)
{
    // Mass
    in.read((char *)&mass, sizeof(mass));
    // Energy loss
    in.read((char *)&energyLoss, sizeof(energyLoss));
    // Has collision
    in.read((char *)&hasCollision, sizeof(hasCollision));
    // Is trigger
    in.read((char *)&isTrigger, sizeof(isTrigger));
    // HitBox
    in.read((char *)&hitBox, sizeof(hitBox));
    // log(LOG_INFO) << "RIGIDBODY: mass: " << mass << ", EnergyLoss: " << energyLoss << ", isTrigger " << isTrigger << ", hitbox: " << hitBox.x << " " << hitBox.y << " " << hitBox.w << " " << hitBox.h << "\n";
}
template class SpawnerComponent<genericBullet>;

template <typename bulletType>
void SpawnerComponent<bulletType>::saveBin(std::ofstream &out)
{
    // Type
    ComponentType type = COMP_TYPE_SPAWNER;
    out.write((char *)&type, sizeof(type));
    // Bullet
    SpawnerBulletType bType = SPAWNER_BULLET_GENERIC;
    out.write((char *)&bType, sizeof(bType));
    // Shoot offset
    out.write((char *)&shootOffset, sizeof(shootOffset));
    // Cooldown
    out.write((char *)&cooldown, sizeof(cooldown));
    // Lifspan
    out.write((char *)&bulletLifeSpan, sizeof(bulletLifeSpan));
}
template <typename bulletType>
void SpawnerComponent<bulletType>::loadBin(std::ifstream &in)
{
    // Shoot offset
    in.read((char *)&shootOffset, sizeof(shootOffset));
    // Cooldown
    in.read((char *)&cooldown, sizeof(cooldown));
    // Lifespan
    in.read((char *)&bulletLifeSpan, sizeof(bulletLifeSpan));
    // log(LOG_INFO) << "SPAWNER: shootOffset: " << shootOffset.x << ", cooldown " << cooldown << ", bulletLifeSpan " << bulletLifeSpan << "\n";
}

void TextComponent::saveBin(std::ofstream &out)
{
    // Type
    ComponentType type = COMP_TYPE_TEXT;
    out.write((char *)&type, sizeof(type));
    // Text
    int messageLength = path.length();
    out.write((char *)&messageLength, sizeof(messageLength));
    out.write(path.c_str(), messageLength);
    // Color
    out.write((char *)&color, sizeof(color));
    // Font path
    int pathLength = fontPath.length();
    out.write((char *)&pathLength, sizeof(pathLength));
    out.write(fontPath.c_str(), pathLength);
}

void TextComponent::loadBin(std::ifstream &in)
{
    // Text
    int messageLength;
    in.read((char *)&messageLength, sizeof(messageLength));
    path.resize(messageLength);
    in.read(&path[0], messageLength);
    // Color
    in.read((char *)&color, sizeof(color));
    // Font path
    int fontPathLength;
    in.read((char *)&fontPathLength, sizeof(fontPathLength));
    fontPath.resize(fontPathLength);
    in.read(&fontPath[0], fontPathLength);
    // Load image
    setFont(fontPath);
    // log(LOG_INFO) << "TEXT: messageLength: " << messageLength << ", message: " << path.c_str() << "\n";
}

void LayoutHelperComponent::saveBin(std::ofstream &out)
{
    // Comp type
    ComponentType type = COMP_TYPE_LAYOUT;
    out.write((char *)&type, sizeof(type));
    // Layout ID
    out.write((char *)&layoutID, sizeof(layoutID));
    // Element ID
    out.write((char *)&id, sizeof(id));
}

void LayoutHelperComponent::loadBin(std::ifstream &in)
{
    // Layout ID
    in.read((char *)&layoutID, sizeof(layoutID));
    // Element ID
    in.read((char *)&id, sizeof(id));
}
#pragma endregion
#pragma region Savers/Loaders

bool saveBin(Object *obj, std::string filename)
{
    log(LOG_INFO) << "Saving " << obj->getName() << " to binary file " << filename.c_str() << "\n";
    std::ofstream out(filename, std::ios::binary);
    if (!out)
    {
        log(LOG_WARN) << "Failed save for" << obj->getName() << ", " << filename.c_str() << " failed to open\n";
        return false;
    }
    LoadFlag flag = OBJECT;
    if (dynamic_cast<Grid *>(obj))
    {
        flag = GRID;
    }
    else if (dynamic_cast<Object *>(obj))
    {
        flag = OBJECT;
    }
    else
    {
        log(LOG_WARN) << "saveBin failed to detect type of " << obj->getName() << ", defaulting to Object\n";
    }
    out.write((char *)&flag, sizeof(flag));
    obj->saveBin(out);
    out.close();
    return true;
}

Object *loadBin(std::string filename, Scene *scene)
{
    log(LOG_INFO) << "Loading Object from " << filename.c_str() << "\n";
    std::ifstream in(filename, std::ios::binary);
    LoadFlag flag;
    Object *obj = nullptr;
    if (!in)
    {
        log(LOG_WARN) << "Failed to load object from" << filename.c_str() << " (failed to open)\n";
        return nullptr;
    }
    in.read((char *)&flag, sizeof(flag));
    switch (flag)
    {
    case OBJECT:
        obj = new Object(scene);
        break;
    case GRID:
        obj = new Grid(scene);
        break;
    default:
        log(LOG_WARN) << "Failed to load file" << filename.c_str() << ", not save file or file corupted\n";
        return nullptr;
    }
    obj->loadBin(in);
    in.close();
    return obj;
}