#include "Game.h"
#include "Constants.h"
Game::Game(HAPISPACE::BYTE* _screen) :
	m_screen(_screen),
	m_starsAmount(100),
	m_stars(),
	m_currentEyeDistance(1),
	m_testTexture("Data/alphaThing.tga", { 500, 500 }) {
}

void Game::Update() {
	HandleInput();

	for (auto& star : m_stars) {
		star.Update();
	}
}

void Game::Render() {
	m_testTexture.Render(m_screen);
	if (!m_stars.empty()) {
		ClearScreen();
		for (auto& star : m_stars) {
			star.Render(m_screen);
		}
		HAPI.RenderText(constants::k_screenWidth / 5, 0, HAPISPACE::HAPI_TColour::WHITE,
			"EYE DISTANCE : " + std::to_string(m_currentEyeDistance));
		HAPI.RenderText(constants::k_screenWidth / 2, 0, HAPISPACE::HAPI_TColour::WHITE,
			"AMOUNT OF STARS : " + std::to_string(m_starsAmount));
	}
}

void Game::HandleInput() {
	// GENERAL CONTROLS
	if (GetKey(EKeyCode::R)) {
		m_stars.clear();
		ClearScreen(HAPISPACE::HAPI_TColour::RED);
	} else if (GetKey(EKeyCode::G)) {
		m_stars.clear();
		ClearScreen(HAPISPACE::HAPI_TColour::GREEN);
	} else if (GetKey(EKeyCode::B)) {
		m_stars.clear();
		ClearScreen(HAPISPACE::HAPI_TColour::BLUE);
	} else if (GetKey(EKeyCode::Y)) {
		m_stars.clear();
		ClearScreen({ 255, 255, 0, 255 });
	} else if (GetKey(EKeyCode::P)) {
		m_stars.clear();
		SetPixel(constants::rand_range(0, (constants::k_screenWidth - 1) * 4),
			constants::rand_range(0, (constants::k_screenHeight - 1) * 4),
			{ static_cast<unsigned char>(constants::rand_range(0, 255)),
				static_cast<unsigned char>(constants::rand_range(0, 255)),
				static_cast<unsigned char>(constants::rand_range(0, 255)),
				static_cast<unsigned char>(constants::rand_range(0, 255)) });
	} else if (GetKey(EKeyCode::S)) {
		ClearScreen();
		m_stars.resize(m_starsAmount);
	} else if (GetKey(EKeyCode::C)) {
		m_stars.clear();
		ClearScreen();
	}

	// STARS CONTROLS

	else if (GetKey(EKeyCode::UP)) {
		if (m_stars.size() == static_cast<std::vector<Star>::size_type>(m_starsAmount)) {
			m_currentEyeDistance += 0.1f;
			for (auto& star : m_stars) {
				star.Reset();
				star.SetEyeDistance(m_currentEyeDistance);
			}
		}
	} else if (GetKey(EKeyCode::DOWN)) {
		if (m_stars.size() == static_cast<std::vector<Star>::size_type>(m_starsAmount)) {
			if (static_cast<unsigned long long>(m_currentEyeDistance) > 1) {
				m_currentEyeDistance -= 0.1f;
			}
			for (auto& star : m_stars) {
				star.Reset();
				star.SetEyeDistance(m_currentEyeDistance);
			}
		}
	} else if (GetKey(EKeyCode::RIGHT)) {
		m_starsAmount++;
		m_stars.resize(m_starsAmount);
		for (auto& star : m_stars)
			star.Reset();
	} else if (GetKey(EKeyCode::LEFT)) {
		if (m_starsAmount > 10) {
			m_starsAmount--;
			m_stars.resize(m_starsAmount);
			for (auto& star : m_stars)
				star.Reset();
		}
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
