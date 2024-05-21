#define SDL_MAIN_HANDLED
#include "engine.h"
#include "player.h"

int main(int argc, char *args[])
{
	// SETUP
	LOG_INIT_CERR();
	uint32_t previousTime = SDL_GetTicks();
	uint32_t currentTime;
	EngineInit();

	// CREATE PLAYER
	PlayerObject player;

	bool quit = false;
	SDL_Event e;
	double timer = 0.0;
	int deltaCounter = 0;

	// MAIN LOOP
	while (!quit)
	{
		deltaTime = (double)(SDL_GetTicks() - previousTime) / 1000;
		previousTime = SDL_GetTicks();

		EngineUpdateKeyboard();
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
				log(LOG_INFO) << "Quit requested exiting loop\n";
			}
		}
		mainScene->Update();
		// FIXME: Does not show real values rather shows delta after correction
		if (timer < 10)
		{
			timer += deltaTime;
			deltaCounter++;
		}
		else
		{
			log(LOG_INFO) << "Avarege draw time " << timer / (double)deltaCounter << " s. Theoretical frame rate: " << deltaCounter / timer << "fps\n";
			timer = 0.0;
			deltaCounter = 0;
		}

		EngineCapFrames(60);
	}
	EngineClose();
	return 0;
}