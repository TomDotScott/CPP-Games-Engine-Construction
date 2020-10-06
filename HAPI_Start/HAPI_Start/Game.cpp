#include "Game.h"

void Game::ClearScreen(const Colour _colour) const {
	for (int i = 0; i < (m_screenWidth - 1) * 4; i += 4) {
		for (int j = 0; j < (m_screenHeight - 1) * 4; j += 4) {
			SetPixel(i, j, _colour);
		}
	}
}

void Game::SetPixel(const int _x, const int _y, const Colour _colour) const {
	m_screen[m_screenWidth * _x + _y] = _colour.m_r;
	m_screen[m_screenWidth * _x + _y + 1] = _colour.m_g;
	m_screen[m_screenWidth * _x + _y + 2] = _colour.m_b;
	m_screen[m_screenWidth * _x + _y + 3] = _colour.m_a;
}

void Game::SetPixel(const int _x, const int _y, const int _value) const {
	m_screen[m_screenWidth * _x + _y] = _value;
}

void Game::HandleInput() const
{
	auto keyData = HAPI.GetKeyboardData();
	// R = RED
	if (keyData.scanCode[82]) {
		ClearScreen({ 255, 0, 0, 255 });
	}
	// G = GREEN
	else if (keyData.scanCode[71]) {
		ClearScreen({ 0, 255, 0, 255 });
	}
	// B = BLUE
	else if (keyData.scanCode[66]) {
		ClearScreen({ 0, 0, 255, 255 });
	}
	// Y = YELLOW
	else if (keyData.scanCode[89]) {
		ClearScreen({ 255, 255, 0, 255 });
	}
	// P = Random Pixel to Random Colour
	else if (keyData.scanCode[80]) {
		SetPixel(RandRange(0, (m_screenWidth - 1) * 4), 
				RandRange(0, (m_screenHeight - 1) * 4), 
			{ RandRange(0, 255), RandRange(0, 255), RandRange(0, 255), RandRange(0, 255) });
	}
	// C = CLEAR THE SCREEN
	else if (keyData.scanCode[67]) {
		ClearScreen();
	}
}
