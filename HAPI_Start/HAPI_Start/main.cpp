#include <cstdlib>
#include <ctime>
#include <HAPI_lib.h>
#include "Game Components/Game.h"
#include "State System/StateManager.h"


StateManager STATE_MANAGER{};

void HAPI_Main() {
	srand(static_cast<unsigned>(time(nullptr)));

	int width = constants::k_screenWidth;
	int height = constants::k_screenHeight;

	if (!HAPI.Initialise(width, height, "Nano's Adventure"))
	{
		HAPI.Close();
	}

	HAPI.SetShowFPS(true);

	STATE_MANAGER.OnCreate(eState::e_MainMenu);
	
	while (HAPI.Update()) {
		//const float dt = delta_time(gameClock);

		STATE_MANAGER.Update();
		
		STATE_MANAGER.Render();
		
		// Reset the clock
		//gameClock = clock();
	}
}
