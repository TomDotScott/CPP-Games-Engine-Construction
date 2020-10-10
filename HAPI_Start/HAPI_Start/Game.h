#pragma once
#include <HAPI_lib.h>
#include "Star.h"
#include <array>

struct Colour {
	Colour(const int _r, const int _g, const int _b, const int _a) :
		m_r(_r),
		m_g(_g),
		m_b(_b),
		m_a(_a) {
	};
	unsigned int m_r, m_g, m_b, m_a;
};

enum class EKeyCode {
	None = 0, LEFT = 37, UP = 38, RIGHT = 39, DOWN = 40, B = 66, C = 67, G = 71, P = 80, R = 82, S = 83, Y = 89
};

class Game {
public:
	explicit Game(HAPISPACE::BYTE& _screen, const int _width = 1000, const int _height = 1000);

	void Update();
	void Render();
	void HandleInput();

	void ClearScreen(Colour _colour) const;
	void ClearScreen() const;

	void SetPixel(int _x, int _y, Colour _colour) const;
	void SetPixel(int _x, int _y, int _value) const;

private:
	static int RandRange(const int _min, const int _max) {
		return _min + (rand() % (_max - _min + 1));
	}

	const std::array<int, 6> m_eyeDistances {
		10,
		20,
		30,
		50,
		100,
		200,
	};

	bool GetKey(EKeyCode _keyCode);
	bool GetKeyDown(EKeyCode _keyCode);

	HAPISPACE::BYTE& m_screen;
	int m_screenWidth;
	int m_screenHeight;

	int m_starsAmount;
	std::vector<Star> m_stars;
	int m_eyeDistanceIndex;
	EKeyCode m_lastKeyPressed;
};