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
	Player m_player1;
	Player m_player2;
	Ball m_pongBall;
	clock_t m_gameClock;
	float m_countDownTimer;
	bool m_gameStarted;

	void CountDown();
	bool GetKey(EKeyCode keyCode) const;
	void HandleKeyBoardInput();
	void HandleControllerInput();
};

enum class EKeyCode {
	None = 0, SPACE = 32, LEFT = 37, UP = 38, RIGHT = 39, DOWN = 40, B = 66, C = 67, G = 71, P = 80, R = 82, S = 83, W = 87, Y = 89
};
