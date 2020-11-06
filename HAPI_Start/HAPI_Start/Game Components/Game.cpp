#include "Game.h"
#include "../Graphics/Graphics.h"
Game::Game() :
	m_keyboardData(HAPI.GetKeyboardData()),
	m_controllerData(HAPI.GetControllerData(0)),
	m_gameScore(),
	m_player("Data/alphaThing.tga",
		"Player",
		{ static_cast<float>(constants::k_screenWidth) / 2, constants::k_screenHeight }
	),
	m_ball("Data/ball.tga",
		"Ball",
		{ static_cast<float>(constants::k_screenWidth) / 2, static_cast<float>(constants::k_screenHeight) / 2 },
		{ static_cast<float>(constants::rand_range(1, 2)) }
	),
	m_gameClock(),
	m_countDownTimer(3) {
	Graphics::GetInstance().CreateTexture("Data/pongBackground.tga", "Background");
	Graphics::GetInstance().CreateSpriteSheet("Data/SpriteSheet.tga", 64);
	Graphics::GetInstance().SetSpriteSheetLocation("Sheet", {2,3});
}

void Game::Update() {
	HandleKeyBoardInput();
	HandleControllerInput();
	m_player.Update(m_gameClock);
	m_ball.Update(m_gameClock);
	m_gameClock = clock();
}

void Game::Render() const {
	Graphics::GetInstance().ClearScreen();
	// Graphics::GetInstance().DrawTexture("Background", { 0, 0 });
	
	Vector2 centreOfScreen{constants::k_screenWidth / 2, constants::k_screenHeight / 2};
	Graphics::GetInstance().DrawSprite("Sheet", centreOfScreen);
	
	//m_player.Render();
	// m_ball.Render();

	//// Only render the timer if we need to
	//if (m_ball.GetBallInPlay() == false) {
	//	HAPI.RenderText((constants::k_screenWidth / 2) - (constants::k_screenHeight - 200) / 4,
	//		(constants::k_screenHeight / 2) - ((constants::k_screenHeight - 200) / 2) - 50,
	//		HAPISPACE::HAPI_TColour::WHITE,
	//		HAPISPACE::HAPI_TColour::BLACK,
	//		3,
	//		std::to_string(static_cast<int>(m_countDownTimer)),
	//		constants::k_screenHeight - 200,
	//		HAPISPACE::eItalic
	//	);
	//}
}

void Game::HandleKeyBoardInput() {
	// Player One Controls
	if (GetKey(EKeyCode::W)) {
		m_player.SetDirection(Vector2::UP);
	} else if (GetKey(EKeyCode::S)) {
		m_player.SetDirection(Vector2::DOWN);
	}else if (GetKey(EKeyCode::A)) {
		m_player.SetDirection(Vector2::LEFT);
	} else if (GetKey(EKeyCode::D)) {
		m_player.SetDirection(Vector2::RIGHT);
	} else {
		m_player.SetDirection(Vector2::ZERO);
	}
}

void Game::HandleControllerInput() {
	//Player One Controls 
	Vector2 leftStickVector{ static_cast<float>(m_controllerData.analogueButtons[2]), static_cast<float>(m_controllerData.analogueButtons[3]) };
	if (leftStickVector.Magnitude() > constants::k_leftThumbDeadzone) {
		leftStickVector.Normalised();

		if (leftStickVector.y > 0) {
			m_player.SetDirection(Vector2::UP);
		} else if (leftStickVector.y < 0) {
			m_player.SetDirection(Vector2::DOWN);
		} else {
			m_player.SetDirection(Vector2::ZERO);
		}
	}
}

bool Game::GetKey(const EKeyCode keyCode) const {
	return m_keyboardData.scanCode[static_cast<int>(keyCode)] ? true : false;
}
