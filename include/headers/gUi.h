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
    void setColorSimple(Uint8 r, Uint8 g, Uint8 b, Uint8 a) { setColor({r, g, b, a}); };
    void setOffset(Vect newOffset) { offset = newOffset; };

protected:
    int resolution;
    SDL_Vertex getSpherePosition(int index);
    int *indexes;
    int *generateIndexes();
    int nrOfIndexes;
    float radius;
    SDL_Vertex *vertices;
    Vect offset;

private:
    SDL_Color color;
};

class uiRoundedRect : public uiSphere
{
public:
    uiRoundedRect(Object *parent, int resolution, float radius, float width, float height);
    void render() override;
    void setWidth(float newWidth) { width = newWidth; };
    void setHeight(float newHeight) { height = newHeight; };
    Vect getSize() { return {width, height}; };

private:
    float width;
    float height;
    Vect center;
};

class uiButton : public Object
{
public:
    /*Creates rounded button @param scene scene pointer @param resolution for rounded corner @param radius for rounded corners @param width @param height @param borderSize if 0 no border @param text if empty no text*/
    uiButton(Scene *scene, int resolution, float radius, float width, float height, float borderSize = 0, std::string text = "");
    void update() override;
    void setText(std::string newText) { buttonText = newText; };
    void setColor(SDL_Color setColor, float borderShift);
    Vect getSize() { return body->getSize(); };
    /* Runs once when button changes states */
    bool buttonClicked();
    void setOnClick(std::function<void()> func) { onClick = func; };

private:
    std::function<void()> onClick;

    uiRoundedRect *body;
    uiRoundedRect *border;
    std::string buttonText;

    SDL_Color hoverColor;
    SDL_Color color;
    SDL_Color borderColor;
    float borderColorShift;

    bool hover;
    bool prevHover;
    bool clicked;
    bool prevClicked;
    int fontSize;
    SDL_Color fontColor;
};
