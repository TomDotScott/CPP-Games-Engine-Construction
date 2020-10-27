#include "Game.h"
#include "../Graphics/Graphics.h"
Game::Game() :
	m_gameScore(),
	m_player1("Data/playerOne.tga",
		"Player1",
		{ 100.f, static_cast<float>(constants::k_screenHeight) / 2 }),
	m_player2("Data/playerTwo.tga",
		"Player2",
		{ constants::k_screenWidth - 164.f,
			static_cast<float>(constants::k_screenHeight) / 2 }),
	m_pongBall("Data/ball.tga",
		"Ball",
		{static_cast<float>(constants::k_screenWidth) / 2, static_cast<float>(constants::k_screenHeight) / 2 },
		{ static_cast<float>(constants::rand_range(1, 2)) },
		m_player1,
		m_player2,
		m_gameScore),
	m_gameClock(),
	m_countDownTimer(3),
	m_gameStarted(false) {
	Graphics::GetInstance().CreateTexture("Data/pongBackground.tga", "Background");
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
	Graphics::GetInstance().ClearScreen();	
	Graphics::GetInstance().DrawTexture("Background", { 0, 0 });

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

	m_player1.Render();
	m_player2.Render();
	m_pongBall.Render();

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
	if (leftStickVector.Magnitude() > constants::k_leftThumbDeadzone) {
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
	if (rightStickVector.Magnitude() > constants::k_rightThumbDeadzone) {
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
