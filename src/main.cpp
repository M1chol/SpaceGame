#define SDL_MAIN_HANDLED
#include "engine.h"
#include "player.h"
#include "targetObject.h"
#include "mainMenu.h"

int main(int argc, char *args[])
{
	// SETUP
	uint32_t previousTime = SDL_GetTicks();
	uint32_t currentTime;
	EngineInit();

	// Setup Scenes

	Scene *mainScene = addScene("MAIN");

	// CREATE PLAYER
	// PlayerObject *player = new PlayerObject(mainScene);

	// CREATE MENU
	MainMenu *menu = new MainMenu();

	// CREATE TARGETS
	// targetObject *target1 = new targetObject(mainScene, 1);
	// targetObject *target2 = new targetObject(mainScene, 2);
	// targetObject *target3 = new targetObject(mainScene, 3);
	// targetObject *target4 = new targetObject(mainScene, 4);
	// targetObject *target5 = new targetObject(mainScene, 5);
	// targetObject *target6 = new targetObject(mainScene, 6);

	bool quit = false;
	SDL_Event e;
	double timer = 0.0;

	// MAIN LOOP
	while (!quit)
	{
		currentTime = SDL_GetTicks();
		deltaTime = (double)(currentTime - previousTime) / 1000;

		EngineUpdateEvents();

		EngineUpdateScenes();

		drawTime = (double)(SDL_GetTicks() - currentTime) / 1000.0;

		previousTime = currentTime;
		EngineCapFrames(60);
		if (timer < 5)
		{
			timer += deltaTime;
		}
		else
		{
			log(LOG_INFO) << "Current frame rate " << (double)(1 / deltaTime) << " fps. Nr of objects: " << mainScene->getNrOfObjects() << '\n';
			timer = 0.0;
		}
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
				log(LOG_INFO) << "Quit requested...\n";
			}
		}
	}
	EngineClose();
	SDL_Quit();

	log(LOG_INFO) << "SDL Quit successfull!\n";

	if (waitToDebug)
	{
		log(LOG_INFO) << "Waiting for debug is enabled\nPress enter to exit...\n";
		getchar();
	}
	return 0;
}