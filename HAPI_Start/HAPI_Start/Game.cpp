#include "Game.h"

Game::Game(HAPISPACE::BYTE& _screen, const int _width, const int _height) :
	m_screen(_screen),
	m_screenWidth(_width),
	m_screenHeight(_height),
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
		ClearScreen({ 255, 0, 0, 255 });
	} else if (GetKey(EKeyCode::G)) {
		m_lastKeyPressed = EKeyCode::G;

		m_stars.clear();
		ClearScreen({ 0, 255, 0, 255 });
	} else if (GetKey(EKeyCode::B)) {
		m_lastKeyPressed = EKeyCode::B;

		m_stars.clear();
		ClearScreen({ 0, 0, 255, 255 });

	} else if (GetKey(EKeyCode::Y)) {
		m_lastKeyPressed = EKeyCode::Y;

		m_stars.clear();
		ClearScreen({ 255, 255, 0, 255 });
	} else if (GetKey(EKeyCode::P)) {
		m_lastKeyPressed = EKeyCode::P;

		m_stars.clear();
		SetPixel(RandRange(0, (m_screenWidth - 1) * 4),
			RandRange(0, (m_screenHeight - 1) * 4),
			{ RandRange(0, 255), RandRange(0, 255), RandRange(0, 255), RandRange(0, 255) });
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

			if (m_eyeDistanceIndex < m_eyeDistances.size() - 1) {
				m_eyeDistanceIndex++;
			}else
			{
				m_eyeDistanceIndex = 0;
			}

			std::cout << "CURRENT EYE DISTANCE : " << m_eyeDistanceIndex << std::endl;

			for (auto& star : m_stars) {
				star.SetEyeDistance(static_cast<float>(m_eyeDistances[m_eyeDistanceIndex]));
			}
		}
	} else {
		m_lastKeyPressed = EKeyCode::None;
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
	memset((&m_screen), 0, static_cast<size_t>(m_screenWidth * m_screenHeight * 4));
}

void Game::SetPixel(const int _x, const int _y, const Colour _colour) const {
	(&m_screen)[m_screenWidth * _x + _y] = _colour.m_r;
	(&m_screen)[m_screenWidth * _x + _y + 1] = _colour.m_g;
	(&m_screen)[m_screenWidth * _x + _y + 2] = _colour.m_b;
	(&m_screen)[m_screenWidth * _x + _y + 3] = _colour.m_a;
}

void Game::SetPixel(const int _x, const int _y, const int _value) const {
	(&m_screen)[m_screenWidth * _x + _y] = _value;
}

bool Game::GetKey(const EKeyCode _keyCode) {
	return HAPI.GetKeyboardData().scanCode[static_cast<int>(_keyCode)] ? true : false;
}

bool Game::GetKeyDown(EKeyCode _keyCode) {
	return GetKey(_keyCode) && m_lastKeyPressed != _keyCode;
}


