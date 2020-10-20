#include <ctime>

#include "Constants.h"
#include "Game.h"

using namespace HAPISPACE;

// Every HAPI program has a HAPI_Main as an entry point
// When this function exits the program will close down
void HAPI_Main() {
	srand(static_cast<unsigned>(time(nullptr)));
	int localScreenWidth{ constants::k_screenWidth };
	int localScreenHeight{ constants::k_screenHeight };
	if (HAPI.Initialise(localScreenWidth, localScreenHeight, "HAPI Pong")) {
		BYTE* screen = HAPI.GetScreenPointer();
		if (screen) {
			HAPI.SetShowFPS(true);
			Game game{ screen };
			while (HAPI.Update()) {
				// HAPI.UserMessage("Window Has Initialised", ":)");
				game.Update();
				game.Render();
			}
		}

	} else {
		std::cout << "ERROR HAS OCCURED" << std::endl;
	}
}