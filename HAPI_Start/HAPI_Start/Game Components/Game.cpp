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
			std::vector<Tile> row;
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

				const auto tileType = atoi(val.c_str());
				auto tt = static_cast<ETileType>(tileType);
				bool canCollide = true;
				std::string tileIdentifier;
				switch (tt) {
				case ETileType::eAir:
					tileIdentifier = "Air";
					canCollide = false;
					break;
				case ETileType::eDirt:
					tileIdentifier = "Dirt";
					break;
				case ETileType::eGrassLeft:
					tileIdentifier = "Grass_Left";
					break;
				case ETileType::eGrassCentre:
					tileIdentifier = "Grass_Centre";
					break;
				case ETileType::eGrassRight:
					tileIdentifier = "Grass_Right";
					break;
				case ETileType::eStoneTop:
					tileIdentifier = "Stone_Top";
					break;
				case ETileType::eStoneCentre:
					tileIdentifier = "Stone_Centre";
					break;
				case ETileType::eStoneLeft:
					tileIdentifier = "Stone_Left";
					break;
				case ETileType::eStoneRight:
					tileIdentifier = "Stone_Right";
					break;
				case ETileType::eFlag:
					tileIdentifier = "Flag_Up_1";
					break;
				case ETileType::eCoinBlock:
					tileIdentifier = "Block_Coin";
					break;
				case ETileType::eBoxedCoinBlock:
					tileIdentifier = "Block_Boxed_Coin";
					break;
				case ETileType::eCrateBlock:
					tileIdentifier = "Block_Crate";
					break;
				case ETileType::eItemBlock:
					tileIdentifier = "Block_Item";
					break;
				case ETileType::eBrickBlock:
					tileIdentifier = "Block_Brick";
					break;
				case ETileType::eBush:
					canCollide = false;
					tileIdentifier = "Bush";
					break;
				case ETileType::eOpenDoorMid:
					tileIdentifier = "Door_Open_Mid";
					break;
				case ETileType::eOpenDoorTop:
					tileIdentifier = "Door_Open_Top";
					break;
				case ETileType::ePlant:
					canCollide = false;
					tileIdentifier = "Plant";
					break;
				case ETileType::eMushroom1:
					canCollide = false;
					tileIdentifier = "Mushroom1";
					break;
				case ETileType::eMushroom2:
					canCollide = false;
					tileIdentifier = "Mushroom2";
					break;
				case ETileType::eRock:
					canCollide = false;
					tileIdentifier = "Rock";
					break;
				case ETileType::eSpikes:
					tileIdentifier = "Spikes";
					break;
				case ETileType::eFlagPole:
					tileIdentifier = "Flag_Pole";
					break;
				default:
					HAPI.UserMessage("Unknown Tile Type: " + std::to_string(tileType), "Error Occured");
					break;
				}				
				row.emplace_back(tileIdentifier, tt, canCollide);
			}
			m_levelData.emplace_back(row);
		}
	}
}

void Game::CheckPlayerLevelCollision(const int chunkNum, Vector2 playerPos) {
	const int playerX = static_cast<int>(playerPos.x);
	const int playerY = static_cast<int>(playerPos.y);

	// Check the bottom of the Player...
	if (playerPos.y < constants::k_chunkHeight - 1) {
		if (m_levelData[playerY + 1][playerX].m_canCollide) {
			m_player.SetIsGrounded(true);
			m_player.SetIsJumping(false);
		} else {
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

void Game::RenderChunk(const int chunkNum) {
	const int colStart = chunkNum * constants::k_chunkWidth;
	const int colEnd = colStart + constants::k_chunkWidth;

	for (int r = 0; r < constants::k_chunkHeight; ++r) {
		for (int c = colStart; c < colEnd; ++c) {
			const auto currentTile = m_levelData[r][c];
			if (currentTile.m_type != ETileType::eAir) {
				Graphics::GetInstance().DrawSprite(
					currentTile.m_spriteIdentifier,
					{
						static_cast<float>((c * constants::k_spriteSheetCellWidth) - chunkNum * constants::k_screenWidth),
						static_cast<float>(r * constants::k_spriteSheetCellWidth)
					}
				);
			}
		}
	}
}

