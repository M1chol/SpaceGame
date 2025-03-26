#include "engine.h"

class uiSphere : public Object
{
public:
    uiSphere(Object *parent, int resolution, float radius);
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
    uiRoundedRect(Object *parent, int resolution, float radius, float width, float height);
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
    /*Creates rounded button @param scene scene pointer @param resolution for rounded corner @param radius for rounded corners @param width @param height @param borderSize if 0 no border @param text if empty no text*/
    uiButton(Scene *scene, int resolution, float radius, float width, float height, float borderSize = 0, std::string text="");
    void update() override;
    void setText(std::string newText) { buttonText = newText; };

private:
    uiRoundedRect *body;
    uiRoundedRect *border;
    SDL_Color color;
    float border_shift;
    std::string buttonText;
};

class MainMenu : public Scene
{
public:
    MainMenu();

private:
    Object *title;
};