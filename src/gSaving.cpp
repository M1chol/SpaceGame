#include <engine.h>

Component *loadAndAttach(std::ifstream &in, Object *obj);

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
    log(LOG_INFO) << "Saved " << name << " with " << nrOfComponents << " Components\n";
}
void Object::loadBin(std::ifstream &in)
{
    // Pos
    in.read((char *)&pos, sizeof(pos));
    // Scene by scene id
    int sceneID = getSceneID(linkedScene);
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
    log(LOG_INFO) << "Loaded " << name << " (" << this << ") with " << nrOfComponents << " Components\n";
}

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