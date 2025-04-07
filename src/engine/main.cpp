#include "engine.h"
#include <SDL.h>
#include <cstdio> // For getchar()

int main(int argc, char *args[])
{
	// Get the engine singleton instance.
	gEngine &eng = gEngine::getInstance();

	// Setup time tracking.
	uint32_t previousTime = SDL_GetTicks();
	uint32_t currentTime = 0;

	// Initialize the engine.
	if (!eng.init())
	{
		log(LOG_ERR) << "Failed to initialize engine.\n";
		return -1;
	}

	// TEST LUA: Run the test script.
	lua->run("src/test.lua");

	// Setup Scenes: create a scene via the engine interface.
	Scene *mainScene = eng.addScene("MAIN");

	SDL_Event e;
	double timer = 0.0;

	// MAIN LOOP
	while (eng.isRunning())
	{
		currentTime = SDL_GetTicks();
		deltaTime = static_cast<double>(currentTime - previousTime) / 1000.0;

		// Update engine events (keyboard, mouse, etc.)
		eng.updateEvents();

		// Update scenes.
		eng.updateScenes();

		// Calculate draw time.
		drawTime = static_cast<double>(SDL_GetTicks() - currentTime) / 1000.0;

		previousTime = currentTime;
		eng.capFrames(60); // Cap the frame rate at 60 FPS.

		if (timer < 5.0)
		{
			timer += deltaTime;
		}
		else
		{
			log(LOG_INFO) << "Current frame rate " << (1.0 / deltaTime)
						  << " fps. Nr of objects: " << mainScene->getNrOfObjects()
						  << "\n";
			timer = 0.0;
		}

		// Process SDL events.
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				eng.requestClose();
				log(LOG_INFO) << "Quit requested...\n";
			}
		}
	}

	// Close the engine and clean up resources.
	eng.close();
	log(LOG_INFO) << "SDL Quit successful!\n";

	if (waitToDebug)
	{
		log(LOG_INFO) << "Waiting for debug is enabled\nPress enter to exit...\n";
		getchar();
	}
	return 0;
}
