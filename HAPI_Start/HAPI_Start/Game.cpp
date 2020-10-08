#include "Game.h"

void Game::Update() {
	HandleInput();
	if (m_runStars) {
		for (auto& star : m_stars) {
			star.Update();
		}
	}
}

void Game::Render() {
	if (m_runStars) {
		ClearScreen();
		for (auto& star : m_stars) {
			star.Render(m_screen);
		}
	}
}

void Game::HandleInput() {
	auto currentKeyData = HAPI.GetKeyboardData();
	// R = RED
	if (currentKeyData.scanCode[82]) {
		m_runStars = false;
		ClearScreen({ 255, 0, 0, 255 });
	}
	// G = GREEN
	if (currentKeyData.scanCode[71]) {
		m_runStars = false;
		ClearScreen({ 0, 255, 0, 255 });
	}
	// B = BLUE
	if (currentKeyData.scanCode[66]) {
		m_runStars = false;
		ClearScreen({ 0, 0, 255, 255 });
	}
	// Y = YELLOW
	if (currentKeyData.scanCode[89]) {
		m_runStars = false;
		ClearScreen({ 255, 255, 0, 255 });
	}
	// P = Random Pixel to Random Colour
	if (currentKeyData.scanCode[80]) {
		m_runStars = false;
		SetPixel(RandRange(0, (m_screenWidth - 1) * 4),
			RandRange(0, (m_screenHeight - 1) * 4),
			{ RandRange(0, 255), RandRange(0, 255), RandRange(0, 255), RandRange(0, 255) });
	}
	// S = STARS SIMULATION
	if (currentKeyData.scanCode[83]) {
		ClearScreen();

		for (auto& star : m_stars)
		{
			star.Reset();
		}

		m_runStars = true;
	}
	// C = CLEAR THE SCREEN
	if (currentKeyData.scanCode[67]) {
		ClearScreen();
	}
}

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


