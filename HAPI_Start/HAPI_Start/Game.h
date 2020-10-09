#pragma once
#include <HAPI_lib.h>
#include "Star.h"

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
	R = 82, G = 71, B = 66, Y = 89, P = 80, S = 83, C = 67
};

class Game {
public:
	explicit Game(HAPISPACE::BYTE* _screen, const int _width = 1000, const int _height = 1000);

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

	HAPISPACE::BYTE* m_screen;
	int m_screenWidth;
	int m_screenHeight;

	int m_starsAmount;
	std::vector<Star> m_stars;
};