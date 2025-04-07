#define SDL_MAIN_HANDLED
#include "engine.h"

void loadGame();

int main(int argc, char *args[])
{
	// SETUP
	uint32_t previousTime = SDL_GetTicks();
	uint32_t currentTime;
	EngineInit();

	// TEST LUA
	lua->run("src/test.lua");

	// Setup Scenes
	Scene *mainScene = new Scene("MAIN");

	SDL_Event e;
	double timer = 0.0;

	// MAIN LOOP
	while (gameLoop)
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
				gameLoop = false;
				log(LOG_INFO) << "Quit requested...\n";
			}
		}
	}
	EngineClose();
	log(LOG_INFO) << "SDL Quit successfull!\n";

	if (waitToDebug)
	{
		log(LOG_INFO) << "Waiting for debug is enabled\nPress enter to exit...\n";
		getchar();
	}
	return 0;
}