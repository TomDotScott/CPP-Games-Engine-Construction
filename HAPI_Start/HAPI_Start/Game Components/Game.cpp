#include "Game.h"
#include "../Graphics/Graphics.h"
#include <fstream>

Game::Game() :
	PLAYER_WON(false),
	PLAYER_LOST(false),
	m_keyboardData(HAPI.GetKeyboardData()),
	m_controllerData(HAPI.GetControllerData(0)),
	m_player({ Vector2::CENTRE }),
	m_gameClock(),
	m_gameScore(0),
	m_currentSprite(0),
	m_backgroundPosition(Vector2::ZERO) {
	if (!Initialise()) {
		HAPI.Close();
	}
}

void Game::Update() {
	// Handle Inputs
	const float deltaTime = DeltaTime();

	HandleKeyBoardInput();
	HandleControllerInput();

	m_player.Update(deltaTime);

	std::cout << m_player.GetPosition() << std::endl;
	
	const float playerOffset = m_player.GetPosition().x;
	for (auto& enemy : m_enemies) {
		// Only update enemies if they're onscreen
		if (enemy.GetPosition().x + (static_cast<float>(constants::k_screenWidth) / 2.f) - playerOffset < constants::k_screenWidth) {
			enemy.Update(deltaTime);
			CheckEnemyLevelCollisions(enemy);

			// Check collisions against the player
			if(enemy.CheckCollisions(m_player.GetGlobalBounds())) {
				enemy.SetEnemyState(EEnemyState::eDead);
			}
		}
	}

	CheckPlayerLevelCollisions(m_player.GetPosition());

	// Scroll the background
	if (m_player.GetCurrentDirection() == Direction::eRight) {
		m_backgroundPosition.x -= 1.f;
		if (m_backgroundPosition.x < -constants::k_screenHeight) {
			m_backgroundPosition.x = 0;
		}
	} else if (m_player.GetCurrentDirection() == Direction::eLeft) {
		m_backgroundPosition.x += 1.f;
		if (m_backgroundPosition.x > constants::k_screenHeight) {
			m_backgroundPosition.x = 0;
		}
	}

	// Reset the clock
	m_gameClock = clock();
}

void Game::Render() {
	Graphics::GetInstance().ClearScreen();

	Graphics::GetInstance().DrawTexture("Background", m_backgroundPosition);
	Graphics::GetInstance().DrawTexture("Background", { m_backgroundPosition.x + constants::k_backgroundTileWidth, 0 });
	Graphics::GetInstance().DrawTexture("Background", { m_backgroundPosition.x + 2 * constants::k_backgroundTileWidth, 0 });

	const float playerXOffset = m_player.GetPosition().x;

	DrawTiles(static_cast<int>(playerXOffset));

	for (auto& enemy : m_enemies) {
		enemy.Render(playerXOffset);
	}

	m_player.Render();
}

void Game::CreateSprite(const std::string& spriteSheetIdentifier) {
	if (!Graphics::GetInstance().CreateSprite(spriteSheetIdentifier, m_currentSprite)) {
		HAPI.Close();
	}
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
		if (m_player.GetCurrentPlayerState() != EPlayerState::eJumping) {
			m_player.SetShouldJump(true);
		}
	}

	Vector2 playerMoveDir = Vector2::ZERO;
	if (GetKey(EKeyCode::A) || GetKey(EKeyCode::LEFT)) {
		if (m_playerMoveLimit != Vector2::LEFT) {
			playerMoveDir = playerMoveDir + Vector2::LEFT;
		}
	} else if (GetKey(EKeyCode::D) || GetKey(EKeyCode::RIGHT)) {
		if (m_playerMoveLimit != Vector2::RIGHT) {
			playerMoveDir = playerMoveDir + Vector2::RIGHT;
		}
	}

	if (playerMoveDir == Vector2::RIGHT) {
		m_player.SetDirection(Direction::eRight);
	} else if (playerMoveDir == Vector2::LEFT) {
		m_player.SetDirection(Direction::eLeft);
	} else {
		m_player.SetDirection(Direction::eNone);
	}
}

void Game::HandleControllerInput() {
	//Player One Controls 
	Vector2 leftStickVector{ static_cast<float>(m_controllerData.analogueButtons[2]), static_cast<float>(m_controllerData.analogueButtons[3]) };
	if (leftStickVector.Magnitude() > constants::k_leftThumbDeadzone) {
		leftStickVector.Normalised();
		if (leftStickVector.x > 0) {
			m_player.SetDirection(Direction::eRight);
		} else if (leftStickVector.x < 0) {
			m_player.SetDirection(Direction::eLeft);
		} else {
			m_player.SetDirection(Direction::eNone);
		}
	}
}

bool Game::Initialise() {
	if (!Graphics::GetInstance().CreateTexture("Data/PlatformerBackground.tga", "Background")) {
		return false;
	}
	if (!Graphics::GetInstance().CreateSpriteSheet("Data/GameSpriteSheet.tga")) {
		return false;
	}

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
	CreateSprite("Player_Jump_Top_1");
	CreateSprite("Player_Jump_Body_1");
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
	if (!LoadLevel()) {
		return false;
	}

	// TODO: Read enemy starting positions from a file and initialise all enemies from that.	
	m_enemies.emplace_back(Vector2(1408, constants::k_screenHeight - 3 * constants::k_spriteSheetCellWidth), true);
	m_enemies.emplace_back(Vector2(2688, constants::k_screenHeight - 3 * constants::k_spriteSheetCellWidth), true);
	m_enemies.emplace_back(Vector2(3968, constants::k_screenHeight - 3 * constants::k_spriteSheetCellWidth), true);
	m_enemies.emplace_back(Vector2(5568, constants::k_screenHeight - 7 * constants::k_spriteSheetCellWidth));
	m_enemies.emplace_back(Vector2(5376, constants::k_screenHeight - 7 * constants::k_spriteSheetCellWidth));
	m_enemies.emplace_back(Vector2(6080, constants::k_screenHeight - 3 * constants::k_spriteSheetCellWidth), true);
	m_enemies.emplace_back(Vector2(6144, constants::k_screenHeight - 3 * constants::k_spriteSheetCellWidth), true);
	m_enemies.emplace_back(Vector2(7232, constants::k_screenHeight - 3 * constants::k_spriteSheetCellWidth), true);
	m_enemies.emplace_back(Vector2(7552, constants::k_screenHeight - 3 * constants::k_spriteSheetCellWidth), true);
	m_enemies.emplace_back(Vector2(7872, constants::k_screenHeight - 3 * constants::k_spriteSheetCellWidth), true);
	m_enemies.emplace_back(Vector2(8128, constants::k_screenHeight - 3 * constants::k_spriteSheetCellWidth), true);
	m_enemies.emplace_back(Vector2(8320, constants::k_screenHeight - 3 * constants::k_spriteSheetCellWidth), true);
	m_enemies.emplace_back(Vector2(8512, constants::k_screenHeight - 3 * constants::k_spriteSheetCellWidth), true);
	m_enemies.emplace_back(Vector2(11136, constants::k_screenHeight - 3 * constants::k_spriteSheetCellWidth), true);
	m_enemies.emplace_back(Vector2(10880, constants::k_screenHeight - 3 * constants::k_spriteSheetCellWidth), true);


	return true;
}

bool Game::LoadLevel() {
	std::ifstream file("Data/Level1.csv");
	if (!file.is_open()) {
		return false;
	}
	while (!file.eof()) {
		for (int r = 0; r < constants::k_maxTilesVertical; ++r) {
			std::vector<Tile> row;
			std::string line;
			std::getline(file, line);
			if (!file.good()) {
				break;
			}
			std::stringstream iss(line);
			for (int c = 0; c < constants::k_maxTilesHorizontal * 11; ++c) {
				std::string val;
				std::getline(iss, val, ',');
				if (!iss.good()) {
					break;
				}

				const auto tileString = atoi(val.c_str());
				const auto tileType = static_cast<ETileType>(tileString);

				bool canCollide = true;

				if (tileType == ETileType::eAir || tileType == ETileType::ePlant ||
					tileType == ETileType::eRock || tileType == ETileType::eBush ||
					tileType == ETileType::eMushroom1 || tileType == ETileType::eMushroom2) {
					canCollide = false;
				}

				row.emplace_back(tileType, canCollide);
			}
			m_levelData.emplace_back(row);
		}
	}

	return true;
}

void Game::CheckPlayerLevelCollisions(const Vector2 playerPos) {
	// Player X is midpoint of the square
	// Add half the screen's width to account for level movement on screen
	int playerXTile = ((static_cast<int>(playerPos.x) + (constants::k_spriteSheetCellWidth / 2)) / constants::k_spriteSheetCellWidth) +
		constants::k_maxTilesHorizontal / 2;
	const int playerYTile = static_cast<int>(playerPos.y) / constants::k_spriteSheetCellWidth;

	// Check the bottom of the Player...
	if (playerYTile < constants::k_maxTilesVertical - 1) {
		if (m_levelData[playerYTile - 1][playerXTile].m_canCollide) {
			// Reach the peak of the jump
			if (m_player.GetPosition().y <= static_cast<float>(((playerYTile - 1) * constants::k_spriteSheetCellWidth) +
				constants::k_spriteSheetCellWidth)) {
				m_player.SetVelocity({ m_player.GetVelocity().x });
			}

			Tile& currentTile = m_levelData[playerYTile - 1][playerXTile];
			switch (currentTile.m_type) {
			case ETileType::eCrateBlock:
			case ETileType::eCoinBlock:
				currentTile.m_type = ETileType::eAir;
				currentTile.m_canCollide = false;
				break;
			case ETileType::eBoxedCoinBlock:
				currentTile.m_type = ETileType::eCoinBlock;
			case ETileType::eItemBlock:
				currentTile.m_type = ETileType::eBrickBlock;
				break;
			default:;
			}

		} else if (m_levelData[playerYTile + 1][playerXTile].m_canCollide) {
			m_player.SetPlayerState(EPlayerState::eWalking);
			m_player.SetPosition({ playerPos.x, playerPos.y - (playerPos.y - static_cast<float>(playerYTile) * constants::k_spriteSheetCellWidth) });
		} else {
			m_player.SetPlayerState(EPlayerState::eJumping);
		}
	}

	playerXTile = ((static_cast<int>(playerPos.x)) / constants::k_spriteSheetCellWidth) +
		constants::k_maxTilesHorizontal / 2;

	// Check the right of the player
	m_playerMoveLimit = m_levelData[playerYTile][playerXTile + 1].m_canCollide ? Vector2::RIGHT : Vector2::ZERO;

	playerXTile = ((static_cast<int>(playerPos.x) + constants::k_spriteSheetCellWidth) / constants::k_spriteSheetCellWidth) +
		constants::k_maxTilesHorizontal / 2;
	if (m_playerMoveLimit == Vector2::ZERO) {
		// Check the left of the player
		m_playerMoveLimit = m_levelData[playerYTile][playerXTile - 1].m_canCollide ? Vector2::LEFT : Vector2::ZERO;
	}
}

void Game::CheckEnemyLevelCollisions(Enemy& enemy) {
	const auto enemyPos = enemy.GetPosition();

	const int enemyXTile = ((static_cast<int>(enemyPos.x)) / constants::k_spriteSheetCellWidth) + constants::k_maxTilesHorizontal / 2;

	const int enemyYTile = static_cast<int>(enemyPos.y) / constants::k_spriteSheetCellWidth;

	if (enemyYTile > 0 && enemyYTile < constants::k_maxTilesVertical) {
		// Stop falling if there is a walkable block below
		if (m_levelData[enemyYTile + 1][enemyXTile].m_canCollide) {
			enemy.SetIsFalling(false);
		}

		// Check if there is a block to the left
		if (m_levelData[enemyYTile][enemyXTile].m_canCollide && enemy.GetCurrentDirection() == Direction::eLeft) {
			enemy.SetDirection(Direction::eRight);
		}
		// Check if there is a block to the right
		if (m_levelData[enemyYTile][enemyXTile + 1].m_canCollide && enemy.GetCurrentDirection() == Direction::eRight) {
			enemy.SetDirection(Direction::eLeft);
		}

		// If the enemy can stay on platforms...
		if (m_levelData[enemyYTile + 1][enemyXTile].m_type == ETileType::eAir && enemy.GetCurrentDirection() == Direction::eLeft) {
			if (enemy.CanAvoidEdges()) {
				enemy.SetDirection(Direction::eRight);
			} else {
				enemy.SetIsFalling(true);
			}
		}
		if (m_levelData[enemyYTile + 1][enemyXTile + 1].m_type == ETileType::eAir && enemy.GetCurrentDirection() == Direction::eRight) {
			if (enemy.CanAvoidEdges()) {
				enemy.SetDirection(Direction::eLeft);
			} else {
				enemy.SetIsFalling(true);
			}
		}
	}
}

void Game::DrawTiles(const int playerXOffset) {
	for (size_t y = 0; y < m_levelData.size(); y++) {
		for (size_t x = 0; x < m_levelData[y].size(); x++) {
			const auto currentTile = m_levelData[y][x];
			if (currentTile.m_type != ETileType::eAir) {
				const Vector2 tilePos = {
					static_cast<float>((x * constants::k_spriteSheetCellWidth) - playerXOffset),
					static_cast<float>(y * constants::k_spriteSheetCellWidth)
				};
				if (tilePos.x > 0 && tilePos.x <= constants::k_screenWidth) {
					std::string spriteIdentifier;
					switch (currentTile.m_type) {
					case ETileType::eAir:
						spriteIdentifier = "Air";
						break;
					case ETileType::eDirt:
						spriteIdentifier = "Dirt";
						break;
					case ETileType::eGrassLeft:
						spriteIdentifier = "Grass_Left";
						break;
					case ETileType::eGrassCentre:
						spriteIdentifier = "Grass_Centre";
						break;
					case ETileType::eGrassRight:
						spriteIdentifier = "Grass_Right";
						break;
					case ETileType::eStoneTop:
						spriteIdentifier = "Stone_Top";
						break;
					case ETileType::eStoneCentre:
						spriteIdentifier = "Stone_Centre";
						break;
					case ETileType::eStoneLeft:
						spriteIdentifier = "Stone_Left";
						break;
					case ETileType::eStoneRight:
						spriteIdentifier = "Stone_Right";
						break;
					case ETileType::eFlag:
						spriteIdentifier = "Flag_Up_1";
						break;
					case ETileType::eCoinBlock:
						spriteIdentifier = "Block_Coin";
						break;
					case ETileType::eBoxedCoinBlock:
						spriteIdentifier = "Block_Boxed_Coin";
						break;
					case ETileType::eCrateBlock:
						spriteIdentifier = "Block_Crate";
						break;
					case ETileType::eItemBlock:
						spriteIdentifier = "Block_Item";
						break;
					case ETileType::eBrickBlock:
						spriteIdentifier = "Block_Brick";
						break;
					case ETileType::eBush:
						spriteIdentifier = "Bush";
						break;
					case ETileType::eOpenDoorMid:
						spriteIdentifier = "Door_Open_Mid";
						break;
					case ETileType::eOpenDoorTop:
						spriteIdentifier = "Door_Open_Top";
						break;
					case ETileType::ePlant:
						spriteIdentifier = "Plant";
						break;
					case ETileType::eMushroom1:
						spriteIdentifier = "Mushroom1";
						break;
					case ETileType::eMushroom2:
						spriteIdentifier = "Mushroom2";
						break;
					case ETileType::eRock:
						spriteIdentifier = "Rock";
						break;
					case ETileType::eSpikes:
						spriteIdentifier = "Spikes";
						break;
					case ETileType::eFlagPole:
						spriteIdentifier = "Flag_Pole";
						break;
					default:
						break;
					}
					Graphics::GetInstance().DrawSprite(spriteIdentifier, tilePos);
				}
			}
		}
	}
}
