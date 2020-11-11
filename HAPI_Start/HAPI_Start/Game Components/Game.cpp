#include "Game.h"
#include "../Graphics/Graphics.h"
Game::Game() :
	m_keyboardData(HAPI.GetKeyboardData()),
	m_controllerData(HAPI.GetControllerData(0)),
	m_gameScore(),
	m_player("PlayerLeft",
		{(static_cast<float>(constants::k_screenWidth) / 2) - constants::k_spriteSheetCellWith, constants::k_screenHeight - 150 }
	),
	m_ball("Data/Ball.tga",
		"Ball",
		{ Vector2::CENTRE },
		{0.f, 1.f}
	),
	m_gameClock(),
	m_countDownTimer(3) {
	
	Graphics::GetInstance().CreateTexture("Data/Background.tga", "Background");
	Graphics::GetInstance().CreateSpriteSheet("Data/SpriteSheet.tga", 64);
	
	CreateSprite("PlayerLeft", 0);
	CreateSprite("PlayerRight", 1);
	CreateSprite("GreenBrick", 2);
	CreateSprite("RedBrick", 3);

	int bricksPlaced{ 0 };
	for (int y = 0; y < constants::k_screenHeight / 2; y += constants::k_spriteSheetCellWith) {
		for (int x = 0; x < constants::k_screenWidth; x += constants::k_spriteSheetCellWith) {
			if (bricksPlaced < 20) {
				if (constants::rand_range(1, 100) < 25) {
					// Randomly Assign Red or Green bricks
					if (constants::rand_range(0, 10) < 5) {
						Brick newBrick{ {static_cast<float>(x), static_cast<float>(y)}, EBrickType::eRed };
						m_bricks.push_back(newBrick);
					} else {
						Brick newBrick{ {static_cast<float>(x), static_cast<float>(y)}, EBrickType::eGreen };
						m_bricks.push_back(newBrick);
					}
					bricksPlaced += 1;
				}
			}else {
				return;
			}
		}
	}
}

void Game::Update() {
	// Handle Inputs
	HandleKeyBoardInput();
	HandleControllerInput();

	// Update Objects
	m_player.Update(DeltaTime());
	m_ball.Update(DeltaTime());

	// Check Collisions
	m_ball.CheckCollisions(m_player.GetGlobalBounds(), m_player.GetPosition());
	for (auto& brick : m_bricks) {
		if (brick.GetIsActive() == true) {
			m_ball.CheckCollisions(brick.GetGlobalBounds(), brick.GetPosition());
			brick.CheckCollision(m_ball.GetGlobalBounds());
		}
	}
	
	// Reset the clock
	m_gameClock = clock();
}

void Game::Render() {
	Graphics::GetInstance().ClearScreen();
	Graphics::GetInstance().DrawTexture("Background", { 0, 0 });

	const Vector2 playerPosition = m_player.GetPosition();
	Graphics::GetInstance().DrawSprite("PlayerLeft", playerPosition);
	Graphics::GetInstance().DrawSprite("PlayerRight", { playerPosition.x + constants::k_spriteSheetCellWith, playerPosition.y });
	
	for (auto& brick : m_bricks) {
		if (brick.GetIsActive() == true) {
			Graphics::GetInstance().DrawSprite(brick.GetType() == EBrickType::eRed ? "RedBrick" : "GreenBrick", brick.GetPosition());
		}
	}

	Graphics::GetInstance().DrawTexture("Ball", m_ball.GetPosition());
}

void Game::HandleKeyBoardInput() {
	// Player One Controls
	if (GetKey(EKeyCode::W)) {
		m_player.SetDirection(Vector2::UP);
	} else if (GetKey(EKeyCode::S)) {
		m_player.SetDirection(Vector2::DOWN);
	} else if (GetKey(EKeyCode::A)) {
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
		if (leftStickVector.x > 0) {
			m_player.SetDirection(Vector2::RIGHT);
		} else if (leftStickVector.x < 0) {
			m_player.SetDirection(Vector2::LEFT);
		} else {
			m_player.SetDirection(Vector2::ZERO);
		}
	}
}

void Game::CreateSprite(const std::string& spriteSheetIdentifier, const int spriteSheetLocation) {
	Graphics::GetInstance().CreateSprite(spriteSheetIdentifier, spriteSheetLocation);
}

float Game::DeltaTime() const {
	//CPU "ticks" since the program started.
	const clock_t programTickCount = clock() - m_gameClock;

	//Conversion rate between ticks and milliseconds.
	const float ticksToMilliseconds = 1000.0f / CLOCKS_PER_SEC;

	//Convert from ticks to seconds.
	return programTickCount * ticksToMilliseconds;
}

bool Game::GetKey(const EKeyCode keyCode) const {
	return m_keyboardData.scanCode[static_cast<int>(keyCode)] ? true : false;
}
