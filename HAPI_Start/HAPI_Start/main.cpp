#include <cstdlib>
#include <ctime>
#include <HAPI_lib.h>
#include "Game Components/Game.h"

void HAPI_Main() {
	srand(static_cast<unsigned>(time(nullptr)));
	Game game;
	while (HAPI.Update()) {
		game.Update();
		game.Render();
	}
}
