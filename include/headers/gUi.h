#include "engine.h"

class uiSphere : public Object
{
public:
    uiSphere(Scene *scene, int resolution, float radius);
    ~uiSphere() override
    {
        delete[] vertices;
        delete[] indexes;
    };
    void render() override;
    void setResolution(int newResolution) { resolution = newResolution; };
    void setRadius(float newRadius) { radius = newRadius; };
    void setColor(SDL_Color newColor) { color = newColor; };
    void setRoration(float newRotation) { rotation = newRotation; };
    void setOffset(Vect newOffset) { offset = newOffset; };

protected:
    int resolution;
    SDL_Vertex getSpherePosition(int index);
    int *indexes;
    int *generateIndexes();
    int nrOfIndexes;
    float radius;
    float rotation;
    SDL_Vertex *vertices;
    Vect offset;

private:
    SDL_Color color;
    SDL_Color borderColor;
};

class uiRoundedRect : public uiSphere
{
public:
    uiRoundedRect(Scene *scene, int resolution, float radius, float width, float height);
    void render() override;
    void setWidth(float newWidth) { width = newWidth; };
    void setHeight(float newHeight) { height = newHeight; };

private:
    float width;
    float height;
};

class uiButton : public Object
{
public:
    uiButton(Scene *scene, int resolution, float radius, float width, float height, float borderSize = 0);
    void update() override;

private:
    uiRoundedRect *body;
    uiRoundedRect *border;
    SDL_Color color;
    float border_shift;
};

class MainMenu : public Scene
{
public:
    MainMenu();

private:
    Object *title;
};