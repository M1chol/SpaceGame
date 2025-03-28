#ifndef UIMAINMENU
#define UIMAINMENU

#include "engine.h"

class MainMenu : public Scene
{
public:
    MainMenu();
private:
    Object *title;
    std::vector<Object *> starList;
};

#endif