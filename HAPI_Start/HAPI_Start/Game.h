#pragma once
#include <array>
#include <HAPI_lib.h>
#include "Ball.h"
#include "Player.h"
constexpr float LEFT_THUMB_DEADZONE = 7849.f;
constexpr float RIGHT_THUMB_DEADZONE = 8689.f;


enum class EKeyCode;

class Game {
public:
	explicit Game(HAPISPACE::BYTE* _screen);
	void Update();
	void Render() const;
	void ClearScreen(HAPISPACE::HAPI_TColour _col) const;
	void ClearScreen() const;
	void SetPixel(int _x, int _y, HAPISPACE::HAPI_TColour _colour) const;
	void SetPixel(int _x, int _y, int _value) const;

private:
	HAPISPACE::BYTE* m_screen;
	Score m_gameScore;
	Texture m_gameBackground;
	Player m_player1;
	Player m_player2;
	Ball m_pongBall;

	bool GetKey(EKeyCode _keyCode);
	void HandleKeyBoardInput();
	void HandleControllerInput();
};

enum class EKeyCode {
	None = 0, LEFT = 37, UP = 38, RIGHT = 39, DOWN = 40, B = 66, C = 67, G = 71, P = 80, R = 82, S = 83, W = 87, Y = 89
};
