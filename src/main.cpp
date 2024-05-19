#define SDL_MAIN_HANDLED
#include "engine.h"

int main(int argc, char *args[])
{
	// SETUP
	LOG_INIT_CERR();
	EngineInit();

	bool quit = false;
	SDL_Event e;

	Object player;
	player.pos = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2};
	mainScene->addObject(&player);
	player.addComponent(new SpriteComponent("res/player-placeholder.png"));
	
	// MAIN LOOP
	while (!quit)
	{
		// Check all events
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
				log(LOG_INFO) << "Quit requested exiting loop\n";
			}
			else if (e.type = SDL_KEYDOWN)
			{
				// keyboard check
				switch (e.key.keysym.sym)
				{
				case SDLK_w:
					break;
				case SDLK_a:
					break;
				case SDLK_s:
					break;
				case SDLK_d:
					break;
				default:
					break;
				}
			}
		}
		mainScene->Update();
	}
	EngineClose();
	return 0;
}