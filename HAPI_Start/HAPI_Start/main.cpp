#include <cstdlib>
#include <ctime>
#include <HAPI_lib.h>
#include "Game Components/Game.h"
#include "State System/StateManager.h"

void HAPI_Main() {
	srand(static_cast<unsigned>(time(nullptr)));

	int width = constants::k_screenWidth;
	int height = constants::k_screenHeight;

	if (!HAPI.Initialise(width, height, "Nano's Adventure"))
	{
		HAPI.Close();
	}

	HAPI.SetShowFPS(true);

	StateManager stateManager{};

	stateManager.OnCreate(eState::e_Game);
	
	TextureManager textureManager;

	textureManager.Initialise(HAPI.GetScreenPointer());
	
	while (HAPI.Update()) {
		//const float dt = delta_time(gameClock);

		stateManager.Update();
		
		stateManager.Render(textureManager);
		
		// Reset the clock
		//gameClock = clock();
	}
}
