#include "Game.h"
#include "Constants.h"
Game::Game(HAPISPACE::BYTE* _screen) :
	m_screen(_screen),
	m_starsAmount(100),
	m_stars(),
	m_eyeDistanceIndex(0),
	m_lastKeyPressed(EKeyCode::None) {
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
	// GENERAL CONTROLS
	if (GetKey(EKeyCode::R)) {
		m_lastKeyPressed = EKeyCode::R;

		m_stars.clear();
		ClearScreen(HAPISPACE::HAPI_TColour::RED);
	} else if (GetKey(EKeyCode::G)) {
		m_lastKeyPressed = EKeyCode::G;

		m_stars.clear();
		ClearScreen(HAPISPACE::HAPI_TColour::GREEN);
	} else if (GetKey(EKeyCode::B)) {
		m_lastKeyPressed = EKeyCode::B;

		m_stars.clear();
		ClearScreen(HAPISPACE::HAPI_TColour::BLUE);

	} else if (GetKey(EKeyCode::Y)) {
		m_lastKeyPressed = EKeyCode::Y;

		m_stars.clear();
		ClearScreen({ 255, 255, 0, 255 });
	} else if (GetKeyDown(EKeyCode::P)) {
		m_lastKeyPressed = EKeyCode::P;

		m_stars.clear();
		SetPixel(constants::rand_range(0, (constants::k_screenWidth - 1) * 4),
			constants::rand_range(0, (constants::k_screenHeight - 1) * 4),
			{static_cast<unsigned char>(constants::rand_range(0, 255)),
				static_cast<unsigned char>(constants::rand_range(0, 255)),
				static_cast<unsigned char>(constants::rand_range(0, 255)),
				static_cast<unsigned char>(constants::rand_range(0, 255)) });
	} else if (GetKey(EKeyCode::S)) {
		m_lastKeyPressed = EKeyCode::S;
		ClearScreen();
		m_stars.clear();
		m_stars.resize(m_starsAmount);
	} else if (GetKey(EKeyCode::C)) {
		m_lastKeyPressed = EKeyCode::C;

		m_stars.clear();
		ClearScreen();
	}

	// STARS CONTROLS

	else if (GetKeyDown(EKeyCode::UP)) {
		m_lastKeyPressed = EKeyCode::UP;
		if (m_stars.size() == static_cast<std::vector<Star>::size_type>(m_starsAmount)) {
			if (static_cast<unsigned long long>(m_eyeDistanceIndex) < m_eyeDistances.size() - 1) {
				m_eyeDistanceIndex++;
			} else {
				m_eyeDistanceIndex = 0;
			}

			for (auto& star : m_stars) {
				star.SetEyeDistance(static_cast<float>(m_eyeDistances[m_eyeDistanceIndex]));
			}
		}
	} else {
		m_lastKeyPressed = EKeyCode::None;
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

bool Game::GetKeyDown(const EKeyCode _keyCode) const {
	return GetKey(_keyCode) && m_lastKeyPressed != _keyCode;
}