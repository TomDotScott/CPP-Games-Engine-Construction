#pragma once
#include <ctime>
#include <HAPI_lib.h>
#include "Ball.h"
#include "Player.h"

enum class EKeyCode;

class Game {
public:
	Game();
	void Update();
	void Render() const;

private:
	const HAPISPACE::HAPI_TKeyboardData& m_keyboardData;
	const HAPISPACE::HAPI_TControllerData& m_controllerData;
	Score m_gameScore;
	Player m_player;
	Ball m_ball;
	clock_t m_gameClock;
	float m_countDownTimer;

	bool GetKey(EKeyCode keyCode) const;
	void HandleKeyBoardInput();
	void HandleControllerInput();
};

enum class EKeyCode {
	None = 0, SPACE = 32, LEFT = 37, UP = 38, RIGHT = 39, DOWN = 40, A = 65, B = 66, C = 67, D = 68, G = 71, P = 80, R = 82, S = 83, W = 87, Y = 89
};
