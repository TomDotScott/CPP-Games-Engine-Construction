#pragma once
#include <HAPI_lib.h>
#include "Star.h"

class Game {
public:
	struct Colour {
		Colour(const int _r, const int _g, const int _b, const int _a) :
			m_r(_r),
			m_g(_g),
			m_b(_b),
			m_a(_a) {
		};
		unsigned int m_r, m_g, m_b, m_a;
	};

	explicit Game(HAPISPACE::BYTE* _screen, const int _width = 1000, const int _height = 1000) :
		m_screen(_screen),
		m_screenWidth(_width),
		m_screenHeight(_height) {
		m_runStars = false;
		m_stars.reserve(50);
		for (int i = 0; i < 50; ++i) {
			const Star star{ RandRange(2, 5), {RandRange(0, m_screenWidth), RandRange(0, m_screenHeight), 100}, {0, 0, RandRange(2, 20)} };
			m_stars.push_back(star);
		}
	};

	void Update();
	void Render();

	void HandleInput();

	void ClearScreen(Colour _colour = { 0, 0, 0, 0 }) const;
	void SetPixel(int _x, int _y, Colour _colour) const;
	void SetPixel(int _x, int _y, int _value) const;
private:
	static int RandRange(const int _min, const int _max) {
		return _min + (rand() % (_max - _min + 1));
	}

	HAPISPACE::BYTE* m_screen{ nullptr };
	int m_screenWidth;
	int m_screenHeight;

	// Whether to run the star simulation or not
	bool m_runStars = false;

	std::vector<Star> m_stars;
};