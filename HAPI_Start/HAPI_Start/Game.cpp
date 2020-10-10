#include "Game.h"

Game::Game(HAPISPACE::BYTE* _screen, const int _width, const int _height) :
	m_screen(_screen),
	m_screenWidth(_width),
	m_screenHeight(_height),
	m_starsAmount(50),
	m_stars(){
}

void Game::Update() {
	HandleInput();

	for (auto& star : m_stars) {
		star.Update();
	}

}

void Game::Render() {
	if (!m_stars.empty()) {
		ClearScreen();
		for (auto& star : m_stars) {
			star.Render(m_screen);
		}
	}
}

void Game::HandleInput() {
	auto currentKeyData = HAPI.GetKeyboardData();

	if (currentKeyData.scanCode[static_cast<int>(EKeyCode::R)]) {
		m_stars.clear();
		ClearScreen({ 255, 0, 0, 255 });
	} else if (currentKeyData.scanCode[static_cast<int>(EKeyCode::G)]) {
		m_stars.clear();
		ClearScreen({ 0, 255, 0, 255 });
	} else if (currentKeyData.scanCode[static_cast<int>(EKeyCode::B)]) {
		m_stars.clear();
		ClearScreen({ 0, 0, 255, 255 });
	} else if (currentKeyData.scanCode[static_cast<int>(EKeyCode::Y)]) {
		m_stars.clear();
		ClearScreen({ 255, 255, 0, 255 });
	} else if (currentKeyData.scanCode[static_cast<int>(EKeyCode::P)]) {
		m_stars.clear();
		SetPixel(RandRange(0, (m_screenWidth - 1) * 4),
			RandRange(0, (m_screenHeight - 1) * 4),
			{ RandRange(0, 255), RandRange(0, 255), RandRange(0, 255), RandRange(0, 255) });
	} else if (currentKeyData.scanCode[static_cast<int>(EKeyCode::S)]) {
		ClearScreen();
		m_stars.clear();
		m_stars.resize(m_starsAmount);
	}
	if (currentKeyData.scanCode[static_cast<int>(EKeyCode::C)]) {
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

void Game::ClearScreen() const {
	memset(m_screen, 0, static_cast<size_t>(m_screenWidth * m_screenHeight * 4));
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


