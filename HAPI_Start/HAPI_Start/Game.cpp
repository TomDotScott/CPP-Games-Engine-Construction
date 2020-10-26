#include "Game.h"
#include "Constants.h"
Game::Game(HAPISPACE::BYTE* _screen) :
	m_screen(_screen),
	m_gameScore(),
	m_gameBackground("Data/pongBackground.tga", { 0, 0 }),
	m_player1("Data/playerOne.tga",
		{ 100,
		constants::k_screenHeight / 2 },
		ePlayerNumber::ePlayerOne),
	m_player2("Data/playerTwo.tga",
		{ constants::k_screenWidth - 164,
			constants::k_screenHeight / 2 },
		ePlayerNumber::ePlayerTwo),
	m_pongBall("Data/ball.tga",
		{ constants::k_screenWidth / 2, constants::k_screenHeight / 2 },
		{ static_cast<float>(constants::rand_range(1, 2)) },
		m_player1,
		m_player2,
		m_gameScore),
	m_gameClock(),
	m_countDownTimer(3),
	m_gameStarted(false) {
	HAPI.UserMessage("Press SPACE to start :)", "Press SPACE to start :)");
}

void Game::Update() {
	HandleKeyBoardInput();
	HandleControllerInput();
	if (m_gameStarted) {
		m_player1.Update();
		m_player2.Update();
		m_pongBall.Update();
		if (m_pongBall.GetBallInPlay() == false) {
			CountDown();
		}
	}
	m_gameClock = clock();
}

void Game::Render() const {
	ClearScreen();
	m_gameBackground.Render(m_screen);

	HAPI.RenderText(constants::k_screenWidth / 4,
		constants::k_screenHeight / 2 - 50,
		HAPISPACE::HAPI_TColour::WHITE,
		HAPISPACE::HAPI_TColour::BLACK,
		3,
		std::to_string(m_gameScore.player1Score),
		100,
		HAPISPACE::eBold
	);

	HAPI.RenderText(constants::k_screenWidth - (constants::k_screenWidth / 4) - 100,
		constants::k_screenHeight / 2 - 50,
		HAPISPACE::HAPI_TColour::WHITE,
		HAPISPACE::HAPI_TColour::BLACK,
		3,
		std::to_string(m_gameScore.player2Score),
		100,
		HAPISPACE::eBold
	);

	m_player1.Render(m_screen);
	m_player2.Render(m_screen);
	m_pongBall.Render(m_screen);

	// Only render the timer if we need to
	if (m_pongBall.GetBallInPlay() == false) {
		HAPI.RenderText((constants::k_screenWidth / 2) - (constants::k_screenHeight - 200) / 4,
			(constants::k_screenHeight / 2) - ((constants::k_screenHeight - 200) / 2) - 50,
			HAPISPACE::HAPI_TColour::WHITE,
			HAPISPACE::HAPI_TColour::BLACK,
			3,
			std::to_string(static_cast<int>(m_countDownTimer)),
			constants::k_screenHeight - 200,
			HAPISPACE::eItalic
		);
	}
}

void Game::HandleKeyBoardInput() {
	if (GetKey(EKeyCode::SPACE)) {
		m_gameStarted = true;
	}
	// Player One Controls
	if (GetKey(EKeyCode::W)) {
		m_player1.SetDirection(Vector2::UP);
	} else if (GetKey(EKeyCode::S)) {
		m_player1.SetDirection(Vector2::DOWN);
	} else {
		m_player1.SetDirection(Vector2::ZERO);
	}
	// Player Two Controls	
	if (GetKey(EKeyCode::UP)) {
		m_player2.SetDirection(Vector2::UP);
	} else if (GetKey(EKeyCode::DOWN)) {
		m_player2.SetDirection(Vector2::DOWN);
	} else {
		m_player2.SetDirection(Vector2::ZERO);
	}
}

void Game::HandleControllerInput() {
	auto state = HAPI.GetControllerData(0);
	//Player One Controls 
	Vector2 leftStickVector{ static_cast<float>(state.analogueButtons[2]), static_cast<float>(state.analogueButtons[3]) };
	if (leftStickVector.Magnitude() > LEFT_THUMB_DEADZONE) {
		leftStickVector.Normalised();

		if (leftStickVector.y > 0) {
			m_player1.SetDirection(Vector2::UP);
		} else if (leftStickVector.y < 0) {
			m_player1.SetDirection(Vector2::DOWN);
		} else {
			m_player1.SetDirection(Vector2::ZERO);
		}
	}

	//Player Two Controls 
	Vector2 rightStickVector{ static_cast<float>(state.analogueButtons[4]), static_cast<float>(state.analogueButtons[5]) };
	if (rightStickVector.Magnitude() > RIGHT_THUMB_DEADZONE) {
		rightStickVector.Normalised();

		if (rightStickVector.y > 0) {
			m_player2.SetDirection(Vector2::UP);
		} else if (rightStickVector.y < 0) {
			m_player2.SetDirection(Vector2::DOWN);
		} else {
			m_player2.SetDirection(Vector2::ZERO);
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

void Game::CountDown() {
	if (m_countDownTimer > 0) {
		const clock_t programTickCount = clock() - m_gameClock;
		const float seconds = (1000.f / CLOCKS_PER_SEC) / 1000.f;
		const float deltaTime = (programTickCount * seconds);
		m_countDownTimer -= deltaTime;
	} else {
		m_pongBall.SetBallInPlay(true);
		m_countDownTimer = 3.f;
	}
}

bool Game::GetKey(const EKeyCode _keyCode) {
	return HAPI.GetKeyboardData().scanCode[static_cast<int>(_keyCode)] ? true : false;
}
