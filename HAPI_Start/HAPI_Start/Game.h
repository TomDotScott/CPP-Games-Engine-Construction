#pragma once
#include <HAPI_lib.h>
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
		m_screenWidth(_width),
		m_screenHeight(_height),
		m_screen(_screen) {
	};

	void ClearScreen(Colour _colour = { 0, 0, 0, 0 }) const;
	void SetPixel(int _x, int _y, Colour _colour) const;
	void SetPixel(int _x, int _y, int _value) const;

	static int RandRange(const int _min, const int _max) {
		return _min + (rand() % (_max - _min + 1));
	}
	
	void HandleInput() const;
private:
	int m_screenWidth;
	int m_screenHeight;

	HAPISPACE::BYTE* m_screen{ nullptr };

};



