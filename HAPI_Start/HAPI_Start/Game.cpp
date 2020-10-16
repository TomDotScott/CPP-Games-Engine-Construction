#include "Game.h"
#include "Constants.h"
Game::Game(HAPISPACE::BYTE* _screen) :
	m_screen(_screen),
	m_testTexture("Data/alphaThing.tga", { 500, 500 }),
	m_testBackground("Data/playerSprite.tga", { 500, 500 }) {
}

void Game::Update() {
	HandleInput();
}

void Game::Render() {
	ClearScreen();
	m_testBackground.Render(m_screen);
	m_testTexture.Render(m_screen);
	
}

void Game::HandleInput() {
	// Player One Controls
	if (GetKey(EKeyCode::W)) {

	} else if (GetKey(EKeyCode::S)) {

	}
	// Player Two Controls
	if (GetKey(EKeyCode::UP)) {


	} else if (GetKey(EKeyCode::DOWN)) {


	}
}

void Game::ClearScreen(HAPISPACE::HAPI_TColour _col) const {
	for (int i = 0; i < constants::k_screenWidth * constants::k_screenHeight; i++) {
		memcpy(m_screen + 4 * i, &_col, 4);
	}
}

void Game::ClearScreen() const {
	memset(m_screen, 0, static_cast<size_t>(constants::k_screenWidth * constants::k_screenHeight * 4));
}

void Game::SetPixel(const int _x, const int _y, const HAPISPACE::HAPI_TColour _colour) const {
	m_screen[constants::k_screenWidth * _x + _y] = _colour.red;
	m_screen[constants::k_screenWidth * _x + _y + 1] = _colour.green;
	m_screen[constants::k_screenWidth * _x + _y + 2] = _colour.blue;
	m_screen[constants::k_screenWidth * _x + _y + 3] = _colour.alpha;
}

void Game::SetPixel(const int _x, const int _y, const int _value) const {
	m_screen[constants::k_screenWidth * _x + _y] = _value;
}

bool Game::GetKey(const EKeyCode _keyCode) {
	return HAPI.GetKeyboardData().scanCode[static_cast<int>(_keyCode)] ? true : false;
}
