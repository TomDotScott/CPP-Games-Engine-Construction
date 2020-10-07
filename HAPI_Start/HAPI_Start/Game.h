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
		const Star star{ 10, {20, 30, 10}, {0, 0, 10} };
		m_stars.push_back(star);

		const Star star2{ 50, {200, 300, 10}, {0, 0, 10} };
		m_stars.push_back(star2);


		const Star star3{ 2, {1, 1, 10}, {0, 0, 10} };
		m_stars.push_back(star3);


		const Star star4{ 200, {750, 750, 10}, {0, 0, 10} };
		m_stars.push_back(star4);
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