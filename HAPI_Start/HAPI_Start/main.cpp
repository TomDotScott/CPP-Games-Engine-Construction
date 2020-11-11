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
		if (game.PLAYER_WON) {
			HAPI.UserMessage("You Win!", "You Win");
			HAPI.Close();
		}
		if (game.PLAYER_LOST) {
			HAPI.UserMessage("Game Over! :(", "GAME OVER");
			HAPI.Close();
		}
	}
}
