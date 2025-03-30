#include "engine.h"

#pragma region uiSphere

uiSphere::uiSphere(Object *parent, int newRes, float newRad) : Object(parent)
{
    radius = newRad;
    resolution = newRes;
    offset = {0, 0};
    color = {255, 255, 255, 255};
    vertices = new SDL_Vertex[this->resolution];
    nrOfIndexes = resolution + (resolution - 3) * 2;
    indexes = generateIndexes();
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

#pragma region uiButton

uiButton::uiButton(Scene *scene, int newRes, float radius, float width, float height, float borderSize, std::string text) : Object(scene)
{
    fontColor = {255, 255, 255, 255};
    fontSize = 30;
    borderColorShift = 1.2;
    prevClicked = false;

    if (borderSize > 0)
    {
        border = new uiRoundedRect(this, newRes, radius, width + borderSize, height + borderSize);
        border->setOffset({-borderSize / 2, -borderSize / 2});
        border->move(pos);
    }
    body = new uiRoundedRect(this, newRes, radius, width, height);
    if (text != "")
    {
        Vect center = {width / 2, height / 2};
        TextComponent *textComp = new TextComponent(text, {20, 20}, fontSans, fontSize, this);
        textComp->setColor({255, 255, 255, 255});
        textComp->setOffset(textComp->getCenter() + center.toIVect());
    }
    setColor({37, 150, 190, 255}, borderColorShift);
    body->move(pos);
    log(LOG_INFO) << "uiButton created\n";
}

void uiButton::update()
{
    // TODO: not needed if ui is not moving after creation
    // Object::update();
    // if (border)
    // {
    //     border->move(pos);
    // }
    // body->move(pos);

    Vect size = body->getSize();
    hover = mousePos.x > pos.x && mousePos.x < pos.x + size.x && mousePos.y > pos.y && mousePos.y < pos.y + size.y;

    if (hover && !prevHover)
    {
        body->setColor(hoverColor);
        if (border)
        {
            border->setColor({255, 255, 255, 255});
        }
    }
    else if (!hover && prevHover)
    {
        body->setColor(color);
        if (border)
        {
            border->setColor(borderColor);
        }
    }
    clicked = hover && isKeyDown(MOUSE_LEFT);
    prevClicked = clicked && prevClicked;
    if (buttonClicked())
    {
        if (onClick)
        {
            onClick();
        }
    }
}

void uiButton::setColor(SDL_Color newColor, float borderShift)
{
    color = newColor;
    borderColorShift = borderShift;
    body->setColor(newColor);
    Uint8 R = color.r * borderColorShift > 255 ? 255 : color.r * borderColorShift;
    Uint8 G = color.g * borderColorShift > 255 ? 255 : color.g * borderColorShift;
    Uint8 B = color.b * borderColorShift > 255 ? 255 : color.b * borderColorShift;
    borderColor = {R, G, B, 255};
    if (border)
    {
        border->setColor(borderColor);
    }
    hoverColor = borderColor;
}

bool uiButton::buttonClicked()
{
    if (!prevClicked && clicked)
    {
        prevClicked = true;
        return true;
    }
    return false;
}

#pragma endregion