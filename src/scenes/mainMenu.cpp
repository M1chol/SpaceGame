#include "engine.h"
#include "mainMenu.h"

MainMenu::MainMenu(std::string name) : Scene(name)
{
    setDrawPriority(1);
    Object *background = new Object(this);
    title = new Object(this);
    opacity = 0;
    yOffset = 0;
    fadeSpeed = 0.01;
    moveSpeed = 0.1;
    const double targetY = (double)SCREEN_HEIGHT / 4 - 40;

    title->setName("main-menu-title");
    title->move({(double)SCREEN_WIDTH / 2, (double)SCREEN_HEIGHT / 8});

    TextComponent *text = new TextComponent("SALVAGE SECTOR", {0, 0}, fontVideoBold, 60, title);
    text->setCentered();
    mainColor = {180, 78, 21, 255};

    text->setColor(mainColor);

    startButton = new uiButton(this, 12, 5, 320, 80, 5, "START");
    startButton->move({(double)SCREEN_WIDTH / 2 - startButton->getSize().x / 2, (double)SCREEN_HEIGHT / 4 + 55});
    startButton->setColor(mainColor, 1.4);

    settingsButton = new uiButton(this, 12, 5, 320, 80, 5, "SETTINGS");
    settingsButton->move({(double)SCREEN_WIDTH / 2 - settingsButton->getSize().x / 2, (double)SCREEN_HEIGHT / 4 + 175});
    settingsButton->setColor(mainColor, 1.4);

    quitButton = new uiButton(this, 12, 5, 320, 80, 5, "QUIT");
    quitButton->move({(double)SCREEN_WIDTH / 2 - quitButton->getSize().x / 2, (double)SCREEN_HEIGHT / 4 + 295});
    quitButton->setColor({225, 30, 30, 255}, 1.4);

    // Create stars
    for (int i = 0; i < 100; i++)
    {
        uiSphere *star = new uiSphere(background, 5, rand() % 3 + 1);
        star->move({(double)(rand() % SCREEN_WIDTH), (double)(rand() % SCREEN_HEIGHT)});
    }
}

void MainMenu::update()
{
    // VIBE CODE
    Vect currentPos = title->getPos();

    if (opacity >= 1.0 && currentPos.y >= targetY)
    {

        if (opacity < 1.0)
        {
            opacity += fadeSpeed;
            if (opacity > 1.0)
                opacity = 1.0;
            title->getComponent<TextComponent>()->setColor({mainColor.r, mainColor.g, mainColor.b, (Uint8)(opacity * 255)});
        }

        // Move down
        if (currentPos.y < targetY)
        {
            double step = std::min(moveSpeed, targetY - currentPos.y); // Move towards the target
            title->move({currentPos.x, currentPos.y + step});
        }
    }
}

void MainMenu::setUiState(bool state)
{
    startButton->isActive = state;
    settingsButton->isActive = state;
    quitButton->isActive = state;
}