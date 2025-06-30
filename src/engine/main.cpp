#include "engine.h"
#include <SDL.h>
#include <cstdio>

int main(int argc, char *args[])
{
	// Get the engine singleton instance.
	gEngine &eng = gEngine::getInstance();

	// Initialize the engine.
	if (!eng.init())
	{
		log(LOG_ERR) << "Failed to initialize engine.\n";
		return -1;
	}

	Scene *mainScene = eng.addScene("mainScene");
	eng.lua->run("src/test.lua");

	SDL_Event e;
	double timer = 0.0;

	// MAIN LOOP
	while (eng.isRunning())
	{
		eng.update();

		eng.capFrames(60);

		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				eng.requestClose();
			}
		}
	}

	eng.close();

	if (SETUP_waitToDebug)
	{
		log(LOG_INFO) << "Wait for debug is enabled\nPress enter to exit...\n";
		getchar();
	}
	return 0;
}
