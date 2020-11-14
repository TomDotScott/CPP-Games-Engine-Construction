#include "Game.h"
#include "../Graphics/Graphics.h"
Game::Game() :
	PLAYER_WON(false),
	PLAYER_LOST(false),
	m_keyboardData(HAPI.GetKeyboardData()),
	m_controllerData(HAPI.GetControllerData(0)),
	m_player("Player_Idle_Body_1",
		{ Vector2::CENTRE }
	),
	m_gameClock(),
	m_gameScore(0),
	m_currentSprite(0) {

	Graphics::GetInstance().CreateTexture("Data/PlatformerBackground.tga", "Background");
	Graphics::GetInstance().CreateSpriteSheet("Data/GameSpriteSheet.tga");

	CreateSprite("0");
	CreateSprite("1");
	CreateSprite("2");
	CreateSprite("3");
	CreateSprite("4");
	CreateSprite("5");
	CreateSprite("6");
	CreateSprite("7");
	CreateSprite("8");
	CreateSprite("9");
	CreateSprite("Player_Walk_Top_1");
	CreateSprite("Player_Walk_Body_1");
	CreateSprite("Player_Walk_Top_2");
	CreateSprite("Player_Walk_Body_2");
	CreateSprite("Player_Jump_Top");
	CreateSprite("Player_Jump_Body");
	CreateSprite("Player_Idle_Top_1");
	CreateSprite("Player_Idle_Body_1");
	CreateSprite("Player_Idle_Top_2");
	CreateSprite("Player_Idle_Body_2");
	CreateSprite("Player_Climb_Top_1");
	CreateSprite("Player_Climb_Body_1");
	CreateSprite("Player_Climb_Top_2");
	CreateSprite("Player_Climb_Body_2");
	CreateSprite("Grass_Left");
	CreateSprite("Grass_Centre");
	CreateSprite("Grass_Right");
	CreateSprite("Dirt");
	CreateSprite("Stone_Left");
	CreateSprite("Stone_Centre");
	CreateSprite("Stone_Right");
	CreateSprite("Stone_Top");
	CreateSprite("Coin1");
	CreateSprite("Coin2");
	CreateSprite("Coin3");
	CreateSprite("Flag_Up_1");
	CreateSprite("Flag_Up_2");
	CreateSprite("Flag_Down");
	CreateSprite("Flag_Pole");
	CreateSprite("Gem_1");
	CreateSprite("Gem_2");
	CreateSprite("Gem_3");
	CreateSprite("UI_Heart_Empty");
	CreateSprite("UI_Heart_Half");
	CreateSprite("UI_Heart_Full");
	CreateSprite("UI_Lives");
	CreateSprite("UI_Coins");
	CreateSprite("UI_X");
	CreateSprite("Slime_1");
	CreateSprite("Slime_2");
	CreateSprite("Slime_Squashed");
	CreateSprite("Snail_1");
	CreateSprite("Snail_2");
	CreateSprite("Snail_Shell");
	CreateSprite("Block_Item");
	CreateSprite("Block_Boxed_Coin");
	CreateSprite("Block_Coin");
	CreateSprite("Block_Brick");
	CreateSprite("Block_Crate");
	CreateSprite("Ladder_Top");
	CreateSprite("Ladder_Mid");
	CreateSprite("Arrow_Sign");
	CreateSprite("Bush");
	CreateSprite("Plant");
	CreateSprite("Spikes");
	CreateSprite("Rock");
	CreateSprite("Mushroom1");
	CreateSprite("Mushroom2");
	CreateSprite("Door_Closed_Top");
	CreateSprite("Door_Closed_Mid");
	CreateSprite("Door_Open_Top");
	CreateSprite("Door_Open_Mid");
	CreateSprite("UI_Key_Not_Found");
	CreateSprite("UI_Key_Found");
	CreateSprite("Key");
}

void Game::Update() {
	// Handle Inputs
	HandleKeyBoardInput();
	HandleControllerInput();

	m_player.Update(DeltaTime());

	// Reset the clock
	m_gameClock = clock();
}

void Game::Render() {
	Graphics::GetInstance().ClearScreen();
	Graphics::GetInstance().DrawTexture("Background", { 0, 0 });

	for(int i = 0; i < (constants::k_screenWidth / constants::k_spriteSheetCellWith) + 1; i++) {
		Graphics::GetInstance().DrawSprite("Grass_Centre", {static_cast<float>(i * constants::k_spriteSheetCellWith), constants::k_screenHeight - constants::k_spriteSheetCellWith });
	}
	
	const Vector2 playerPos = m_player.GetPosition();
	Graphics::GetInstance().DrawSprite("Player_Idle_Top_1", { playerPos.x, playerPos.y - constants::k_spriteSheetCellWith });
	Graphics::GetInstance().DrawSprite("Player_Idle_Body_1", playerPos);


}

void Game::HandleKeyBoardInput() {
	if(GetKey(EKeyCode::SPACE)) {
		m_player.SetIsJumping(true);
	}

	
	Vector2 playerMoveDir = Vector2::ZERO;
	if (GetKey(EKeyCode::A) || GetKey(EKeyCode::LEFT)) {
		playerMoveDir = playerMoveDir + Vector2::LEFT;
	}
	if (GetKey(EKeyCode::D) || GetKey(EKeyCode::RIGHT)) {
		playerMoveDir = playerMoveDir + Vector2::RIGHT;
	} 
	m_player.SetDirection(playerMoveDir);
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

void Game::CreateSprite(const std::string& spriteSheetIdentifier) {
	Graphics::GetInstance().CreateSprite(spriteSheetIdentifier, m_currentSprite);
	m_currentSprite++;
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
