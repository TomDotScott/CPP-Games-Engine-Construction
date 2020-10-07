#include "Game.h"

using namespace HAPISPACE;

// Every HAPI program has a HAPI_Main as an entry point
// When this function exits the program will close down
void HAPI_Main() {
	int width{ 1000 };
	int height{ 1000 };
	if (HAPI.Initialise(width, height, "Colour Changing Window!")) {
		BYTE* screen = HAPI.GetScreenPointer();
		HAPI.SetShowFPS(true);
		const Game game{ screen, width, height };
		while (HAPI.Update()) {
			// HAPI.UserMessage("Window Has Initialised", ":)");
			game.HandleInput();
		}
	} else {
		std::cout << "ERROR HAS OCCURED" << std::endl;
	}
}


