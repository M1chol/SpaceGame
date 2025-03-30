#ifndef UIMAINMENU
#define UIMAINMENU

#include "engine.h"

class MainMenu : public Scene
{
public:
    MainMenu(std::string name);
    void update() override;
    void setUiState(bool state);
    uiButton *startButton;
    uiButton *settingsButton;
    uiButton *quitButton;

private:
    SDL_Color mainColor;

    Object *title;
    double opacity;
    double yOffset;
    double fadeSpeed;
    double moveSpeed;
    double targetY;

    std::vector<Object *> starList;
};

#endif