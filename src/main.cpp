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
	PlayerObject player(mainScene);

	bool quit = false;
	SDL_Event e;
	double timer = 0.0;

	// MAIN LOOP
	while (!quit)
	{
		currentTime = SDL_GetTicks();
		deltaTime = (double)(currentTime - previousTime) / 1000;

		EngineUpdateKeyboard();

		mainScene->Update();

		drawTime = (double)(SDL_GetTicks() - currentTime) / 1000.0;

		previousTime = currentTime;
		EngineCapFrames(60);
		if (timer < 5)
		{
			timer += deltaTime;
		}
		else
		{
			log(LOG_INFO) << "Current frame rate " << (int)(1 / deltaTime) << " fps\n";
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

	log(LOG_INFO) << "SDL Quit successfull!";
	return 0;
}