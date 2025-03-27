#include "engine.h"

#pragma region uiSphere

uiSphere::uiSphere(Object *parent, int newRes, float newRad) : Object(parent)
{
    radius = newRad;
    resolution = newRes;
    offset = {0, 0};
    color = {255, 255, 255, 255};
    borderColor = {0, 0, 0, 255};
    vertices = new SDL_Vertex[this->resolution];
    nrOfIndexes = resolution + (resolution - 3) * 2;
    indexes = generateIndexes();
    log(LOG_INFO) << "uiRoundedRect created\n";
}

void uiSphere::render()
{
    Object::render();
    for (int i = 0; i < resolution; i++)
    {
        SDL_Vertex point = getSpherePosition(i);
        point.position.x += (pos.x + offset.x);
        point.position.y += (pos.y + offset.y);
        vertices[i] = point;
    }
    // SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    SDL_RenderGeometry(gRenderer, NULL, vertices, resolution, indexes, nrOfIndexes);
}

SDL_Vertex uiSphere::getSpherePosition(int index)
{
    float angle = (2 * M_PI * index + M_PI) / resolution - rotation / 180 * M_PI;
    return {sin(angle) * radius, cos(angle) * radius, color};
}

int *uiSphere::generateIndexes()
{
    int *indexes = new int[nrOfIndexes];
    int j = 0;
    indexes[j++] = 0;
    indexes[j++] = 1;
    indexes[j++] = 2;

    for (int i = 2; i < resolution - 1; i++)
    {
        indexes[j++] = 0;
        indexes[j++] = i;
        indexes[j++] = i + 1;
    }

    return indexes;
}

#pragma endregion

#pragma region uiRoundedRect

uiRoundedRect::uiRoundedRect(Object *parent, int newRes, float radius, float width, float height) : uiSphere(parent, newRes, radius)
{
    resolution = newRes;
    this->width = width;
    this->height = height;
    center = {width / 2, height / 2};
    log(LOG_INFO) << "uiRoundedRect created\n";
}

void uiRoundedRect::render()
{
    Object::render();

    Vect centers[4] = {
        {width - radius + offset.x, height - radius + offset.y}, // Bottom Right
        {width - radius + offset.x, radius + offset.y},
        {radius + offset.x, radius + offset.y},
        {radius + offset.x, height - radius + offset.y}};

    for (Vect &vec : centers)
        vec = vec.rotate(rotation, center);

    for (int i = 0; i < resolution; i++)
    {
        SDL_Vertex point = getSpherePosition(i);
        point.position.x += pos.x + centers[i * 4 / resolution].x;
        point.position.y += pos.y + centers[i * 4 / resolution].y;
        vertices[i] = point;
    }
    // SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    SDL_RenderGeometry(gRenderer, NULL, vertices, resolution, indexes, nrOfIndexes);
}

#pragma endregion

uiButton::uiButton(Scene *scene, int newRes, float radius, float width, float height, float borderSize, std::string text) : Object(scene)
{
    color = {37, 150, 190, 255};
    border_shift = 0.7;
    borderColor = {static_cast<Uint8>(color.r * border_shift),
                   static_cast<Uint8>(color.g * border_shift),
                   static_cast<Uint8>(color.b * border_shift), 255};
    if (borderSize > 0)
    {
        border = new uiRoundedRect(this, newRes, radius, width + borderSize, height + borderSize);
        border->setOffset({-borderSize / 2, -borderSize / 2});
        border->setColor(borderColor);
        addChild(border);
    }
    body = new uiRoundedRect(this, newRes, radius, width, height);
    body->setColor(color);
    if (text != "")
    {
        TextComponent *textComp = new TextComponent(text, {0, 0}, fontSans, this);
        textComp->setColor({255, 255, 255, 255});
        textComp->setScale(20);
    }
    addChild(body);
    log(LOG_INFO) << "uiButton created\n";
}

void uiButton::update()
{
    Object::update();
    if (border)
    {
        border->move(pos);
    }
    body->move(pos);

    Vect size = body->getSize();
    if (mousePos.x > pos.x && mousePos.x < pos.x + size.x && mousePos.y > pos.y && mousePos.y < pos.y + size.y)
    {
        body->setColor({static_cast<Uint8>(color.r + 20),
                        static_cast<Uint8>(color.g + 20),
                        static_cast<Uint8>(color.b + 20), 255});
        if (border)
        {
            border->setColor({static_cast<Uint8>(color.r + 50),
                              static_cast<Uint8>(color.g + 50),
                              static_cast<Uint8>(color.b + 50), 255});
        }
    }
    else
    {
        body->setColor(color);
        if (border)
        {
            border->setColor(color);
        }
    }
}

MainMenu::MainMenu()
{
    Scene *uiScene = addScene("UI");
    uiScene->setDrawPriority(1);
    Object *title = new Object(uiScene);
    title->setName("title");
    title->move({(double)SCREEN_WIDTH / 2, (double)SCREEN_HEIGHT / 4});
    TextComponent *text = new TextComponent("Scrap Wars", {0, 0}, fontSans, title);
    CustomUpdateComponent *update = new CustomUpdateComponent(title);
    update->setUpdateFunction([](CustomUpdateComponent *comp) -> bool
                              {
        static double angle = 0;
        static bool increasing = true;

        if (increasing)
        {
            angle += .1;
            if (angle >= 10)
            increasing = false;
        }
        else
        {
            angle -= .1;
            if (angle <= -10)
            increasing = true;
        }

        comp->getParent()->rotate(angle);
        return true; });

    text->setScale(30);

    // uiSphere *sphere = new uiSphere(uiScene, 12, 100);
    // sphere->setName("sphere");
    // sphere->move({(double)SCREEN_WIDTH / 2, (double)SCREEN_HEIGHT * 2/ 3});

    uiButton *button = new uiButton(uiScene, 24, 40, 500, 300, 12, "Start");
    button->setName("button");
    button->move({(double)SCREEN_WIDTH / 2, (double)SCREEN_HEIGHT / 2});
};
