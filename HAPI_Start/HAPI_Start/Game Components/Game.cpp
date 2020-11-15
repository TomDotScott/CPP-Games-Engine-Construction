#include "Game.h"
#include "../Graphics/Graphics.h"
#include <fstream>

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
	m_currentSprite(0),
	m_backgroundPosition(Vector2::ZERO) {

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
	CreateSprite("Gem_4");
	CreateSprite("UI_Heart_Empty");
	CreateSprite("UI_Heart_Half");
	CreateSprite("UI_Heart_Full");
	CreateSprite("UI_Lives");
	CreateSprite("UI_Coins");
	CreateSprite("UI_X");
	CreateSprite("Slime_1");
	CreateSprite("Slime_2");
	CreateSprite("Slime_Squashed");
	CreateSprite("Slime_Snail_Shell_Hit");
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
	m_currentChunk = constants::rand_range(0, 9);
	LoadLevel();
}

void Game::Update() {
	// Handle Inputs
	HandleKeyBoardInput();
	HandleControllerInput();

	m_player.Update(DeltaTime());

	const Vector2 playerPos = m_player.GetPosition();

	// See if the player is on the ground
	const Vector2 worldSpacePlayerCoords{
		(playerPos.x / static_cast<float>(constants::k_spriteSheetCellWidth)) + static_cast<float>((m_currentChunk * constants::k_chunkWidth)),
		(playerPos.y / static_cast<float>(constants::k_spriteSheetCellWidth))
	};
	
	CheckPlayerLevelCollision(m_currentChunk, worldSpacePlayerCoords);

	// Reset the clock
	m_gameClock = clock();
}

void Game::Render() {
	Graphics::GetInstance().ClearScreen();
	
	RenderBackground();
	RenderChunk(m_currentChunk);
	
	const Vector2 playerPos = m_player.GetPosition();
	Graphics::GetInstance().DrawSprite("Player_Idle_Top_1", { playerPos.x, playerPos.y - constants::k_spriteSheetCellWidth });
	Graphics::GetInstance().DrawSprite("Player_Idle_Body_1", playerPos);
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

void Game::HandleKeyBoardInput() {
	if (GetKey(EKeyCode::SPACE)) {
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

void Game::LoadLevel() {
	std::ifstream file("Data/Level1.csv");
	while (!file.eof()) {
		for (int r = 0; r < constants::k_chunkHeight; ++r) {
			std::vector<std::string> row;
			std::string line;
			std::getline(file, line);
			if (!file.good()) {
				break;
			}
			std::stringstream iss(line);
			for (int c = 0; c < constants::k_chunkWidth * 11; ++c) {
				std::string val;
				std::getline(iss, val, ',');
				if (!iss.good()) {
					break;
				}
				row.emplace_back(val);
			}
			m_levelData.emplace_back(row);
		}
	}
}

void Game::CheckPlayerLevelCollision(const int chunkNum, Vector2 playerPos) {
	Vector2 difference{ Vector2::ZERO };
	const int playerX = static_cast<int>(playerPos.x);
	const int playerY = static_cast<int>(playerPos.y);
	
	// Check the bottom of the Player...
	if (playerPos.y < constants::k_chunkHeight - 1) {
		// TODO: Make a tile struct with data about collidability... Hard Coding is bad....
		if (m_levelData[playerY + 1][playerX] == "1" ||
			m_levelData[playerY + 1][playerX] == "2" ||
			m_levelData[playerY + 1][playerX] == "3" ||
			m_levelData[playerY + 1][playerX] == "4" ||
			m_levelData[playerY + 1][playerX] == "5" ||
			m_levelData[playerY + 1][playerX] == "6" ||
			m_levelData[playerY + 1][playerX] == "7" ||
			m_levelData[playerY + 1][playerX] == "47" ||
			m_levelData[playerY + 1][playerX] == "48" ||
			m_levelData[playerY + 1][playerX] == "49" ||
			m_levelData[playerY + 1][playerX] == "50" ||
			m_levelData[playerY + 1][playerX] == "51" ||
			m_levelData[playerY + 1][playerX] == "63") {
			
			difference.y -= playerPos.y - static_cast<float>(playerY);
			m_player.SetIsGrounded(true);
			m_player.SetIsJumping(false);
			
		}else {
			m_player.SetIsGrounded(false);
		}
	}
}

void Game::RenderBackground() {
	const Vector2 playerDirection{ m_player.GetCurrentDirection() };
	if (playerDirection != Vector2::ZERO) {
		if (playerDirection.x == Vector2::LEFT.x) {
			m_backgroundPosition.x += 1.f;
			if (m_backgroundPosition.x < -2 * constants::k_screenHeight) {
				m_backgroundPosition.x = 0;
			}
		} else {
			m_backgroundPosition.x -= 1.f;
			if (m_backgroundPosition.x < -constants::k_screenHeight) {
				m_backgroundPosition.x = 0;
			}

		}
	}
	Graphics::GetInstance().DrawTexture("Background", { m_backgroundPosition.x - constants::k_screenHeight, 0 });
	Graphics::GetInstance().DrawTexture("Background", m_backgroundPosition);
	Graphics::GetInstance().DrawTexture("Background", { m_backgroundPosition.x + constants::k_screenHeight, 0 });
}

void Game::RenderChunk(int chunkNum) {
	const int colStart = chunkNum * constants::k_chunkWidth;
	const int colEnd = colStart + constants::k_chunkWidth;

	for (int r = 0; r < constants::k_chunkHeight; ++r) {
		for (int c = colStart; c < colEnd; ++c) {
			const std::string currentTile = m_levelData[r][c];
			if (currentTile != "-1") {
				std::string tileToDraw;
				if (currentTile == "0") {
					tileToDraw = "Dirt";
				} else if (currentTile == "1") {
					tileToDraw = "Grass_Left";
				} else if (currentTile == "2") {
					tileToDraw = "Grass_Centre";
				} else if (currentTile == "3") {
					tileToDraw = "Grass_Right";
				} else if (currentTile == "4") {
					tileToDraw = "Stone_Top";
				} else if (currentTile == "5") {
					tileToDraw = "Stone_Centre";
				} else if (currentTile == "6") {
					tileToDraw = "Stone_Left";
				} else if (currentTile == "7") {
					tileToDraw = "Stone_Right";
				} else if (currentTile == "38") {
					tileToDraw = "Flag_Up_1";
				} else if (currentTile == "47") {
					tileToDraw = "Block_Coin";
				} else if (currentTile == "48") {
					tileToDraw = "Block_Boxed_Coin";
				} else if (currentTile == "49") {
					tileToDraw = "Block_Crate";
				} else if (currentTile == "50") {
					tileToDraw = "Block_Item";
				} else if (currentTile == "51") {
					tileToDraw = "Block_Brick";
				} else if (currentTile == "52") {
					tileToDraw = "Bush";
				} else if (currentTile == "55") {
					tileToDraw = "Door_Open_Mid";
				} else if (currentTile == "56") {
					tileToDraw = "Door_Open_Top";
				} else if (currentTile == "57") {
					tileToDraw = "Plant";
				} else if (currentTile == "60") {
					tileToDraw = "Mushroom1";
				} else if (currentTile == "61") {
					tileToDraw = "Mushroom2";
				} else if (currentTile == "62") {
					tileToDraw = "Rock";
				} else if (currentTile == "63") {
					tileToDraw = "Spikes";
				} else if (currentTile == "64") {
					tileToDraw = "Flag_Pole";
				} else {
					HAPI.UserMessage("Unknown Sprite: " + currentTile, "Error has occured");
				}
				Graphics::GetInstance().DrawSprite(
					tileToDraw,
					{
						static_cast<float>((c * constants::k_spriteSheetCellWidth) - chunkNum * constants::k_screenWidth),
						static_cast<float>(r * constants::k_spriteSheetCellWidth)
					}
				);
			}
		}
	}
}

