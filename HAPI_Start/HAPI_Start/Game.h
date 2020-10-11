#pragma once
#include <HAPI_lib.h>
#include "Star.h"
#include <array>

enum class EKeyCode {
	None = 0, LEFT = 37, UP = 38, RIGHT = 39, DOWN = 40, B = 66, C = 67, G = 71, P = 80, R = 82, S = 83, Y = 89
};

class Game {
public:
	explicit Game(HAPISPACE::BYTE* _screen);

	void Update();
	void Render();
	void HandleInput();

	void ClearScreen(HAPISPACE::HAPI_TColour _col) const;
	void ClearScreen() const;

	void SetPixel(int _x, int _y, HAPISPACE::HAPI_TColour _colour) const;
	void SetPixel(int _x, int _y, int _value) const;

private:
	bool GetKey(EKeyCode _keyCode);

	HAPISPACE::BYTE* m_screen;

	int m_starsAmount;
	std::vector<Star> m_stars;
	float m_currentEyeDistance;
};