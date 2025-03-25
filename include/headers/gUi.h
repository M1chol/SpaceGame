#include "engine.h"

class uiRoundedRect : public Object
{
public:
    uiRoundedRect(Scene *scene, int resolution, float radius);
    void render() override;
    void setText(std::string newValue) {textValue = newValue;};
    void setResolution(int newResolution) {resolution = newResolution;};
private:
    std::string textValue;
    SDL_Vertex getPosition(int index);
    int *generateIndexes();
    int resolution;
    SDL_Vertex *vertices;
    float radius;
    SDL_Color color;
    SDL_Color borderColor;
    int *indexes;
    int nrOfIndexes;
};

class MainMenu : public Scene
{
public:
    MainMenu();

private:
    Object *title;
};