#include "engine.h"
#include "mainMenu.h"

MainMenu::MainMenu()
{
    Scene *uiScene = addScene("main-menu");
    uiScene->setDrawPriority(1);
    Object *background = new Object(uiScene);
    Object *title = new Object(uiScene);

    title->setName("main-menu-title");
    title->move({(double)SCREEN_WIDTH / 2, (double)SCREEN_HEIGHT / 8});

    TextComponent *text = new TextComponent("SALVAGE SECTOR", {0, 0}, fontSans, 60, title);
    text->setCentered();
    static SDL_Color mainColor = {180, 78, 21, 255};

    text->setColor(mainColor);

    uiButton *startButton = new uiButton(uiScene, 12, 5, 320, 80, 5, "START");
    startButton->move({(double)SCREEN_WIDTH / 2 - startButton->getSize().x / 2, (double)SCREEN_HEIGHT / 4 + 55});
    startButton->setColor(mainColor, 1.1);

    uiButton *settingsButton = new uiButton(uiScene, 12, 5, 320, 80, 5, "SETTINGS");
    settingsButton->move({(double)SCREEN_WIDTH / 2 - settingsButton->getSize().x / 2, (double)SCREEN_HEIGHT / 4 + 175});
    settingsButton->setColor(mainColor, 1.1);

    uiButton *quitButton = new uiButton(uiScene, 12, 5, 320, 80, 5, "QUIT");
    quitButton->move({(double)SCREEN_WIDTH / 2 - quitButton->getSize().x / 2, (double)SCREEN_HEIGHT / 4 + 295});
    quitButton->setColor({255, 30, 30, 255}, 1.1);

    CustomUpdateComponent *update = new CustomUpdateComponent(title);
    update->setUpdateFunction([](CustomUpdateComponent *comp) -> bool
                              {
                                  // THIS CODE IS VIBE CODED PROCEED WITH CAUTION
                                  static double opacity = 0.0;
                                  static double yOffset = 0; // Start from off-screen (top)
                                  const double fadeSpeed = 0.01;
                                  const double moveSpeed = 0.4;

                                  // Target position: 1/4 of the screen height
                                  const double targetY = (double)SCREEN_HEIGHT / 4 - 40;

                                  // Fade in
                                  if (opacity < 1.0)
                                  {
                                      opacity += fadeSpeed;
                                      if (opacity > 1.0)
                                          opacity = 1.0;
                                      comp->getParent()->getComponent<TextComponent>()->setColor({mainColor.r, mainColor.g, mainColor.b, (Uint8)(opacity * 255)});
                                  }

                                  // Move down
                                  Vect currentPos = comp->getParent()->getPos();
                                  if (currentPos.y < targetY)
                                  {
                                      double step = std::min(moveSpeed, targetY - currentPos.y); // Move towards the target
                                      comp->getParent()->move({currentPos.x, currentPos.y + step});
                                  }

                                  // Stop animation when both fade-in and movement are complete
                                  return opacity >= 1.0 && currentPos.y >= targetY;

                                  // END OF VIBE CODING
                              });

    // Create stars
    for (int i = 0; i < 100; i++)
    {
        uiSphere *star = new uiSphere(background, 5, rand() % 3 + 1);
        star->move({(double)(rand() % SCREEN_WIDTH), (double)(rand() % SCREEN_HEIGHT)});
    }
}