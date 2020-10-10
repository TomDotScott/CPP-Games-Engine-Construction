#include "Game.h"
#include <ctime>

using namespace HAPISPACE;

// Every HAPI program has a HAPI_Main as an entry point
// When this function exits the program will close down
void HAPI_Main() {
	srand(static_cast<unsigned>(time(nullptr)));
	int width{ 1000 };
	int height{ 1000 };
	if (HAPI.Initialise(width, height, "The Epilepsy Programme")) {
		BYTE* screen = HAPI.GetScreenPointer();
		if (screen) {
			HAPI.SetShowFPS(true);
			Game game{ *screen, width, height };
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