#include "engine.h"

MainMenu::MainMenu()
{
    Scene *uiScene = addScene("UI");
    uiScene->setDrawPriority(1);
    Object *title = new Object(uiScene);
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
            if (angle >= 15)
            increasing = false;
        }
        else
        {
            angle -= .1;
            if (angle <= -15)
            increasing = true;
        }

        comp->getParent()->rotate(angle);
        return true; });

    text->setScale(2.0);

    Object *startButton = new Object(uiScene);
    startButton->move({(double)SCREEN_WIDTH / 2, title->getPos().y + 100});
    TextComponent *startText = new TextComponent("Start", {0, 0}, fontSans, startButton);

    Object *settingsButton = new Object(uiScene);
    settingsButton->move({(double)SCREEN_WIDTH / 2, title->getPos().y + 200});
    TextComponent *settingsText = new TextComponent("Settings", {0, 0}, fontSans, settingsButton);

    Object *quitButton = new Object(uiScene);
    quitButton->move({(double)SCREEN_WIDTH / 2, title->getPos().y + 300});
    TextComponent *quitText = new TextComponent("Quit", {0, 0}, fontSans, quitButton);
};
