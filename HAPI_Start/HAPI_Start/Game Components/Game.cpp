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
	m_backgroundPosition(Vector2::ZERO),
	m_backgroundMoveDir(e_Direction::eNone)
{
	if (!Initialise())
	{
		HAPI.Close();
	}
}

void Game::Update()
{
	const float deltaTime = DeltaTime();

	HandleKeyBoardInput();
	HandleControllerInput();

	m_player.Update(deltaTime);

	for (auto& coin : m_coins)
	{
		coin.Update(deltaTime);
		m_player.CheckEntityCollisions(coin);
		coin.CheckEntityCollisions(m_player);
	}

	UpdateEnemies(m_slimes, deltaTime);
	UpdateEnemies(m_snails, deltaTime);

	CheckPlayerLevelCollisions(m_player.GetCurrentCollisionBoxes());

	// Scroll the background
	if (m_player.GetCurrentDirection() == e_Direction::eRight && m_player.GetMoveDirectionLimit() != e_Direction::eRight)
	{
		m_backgroundPosition.x -= 1.f;
		m_backgroundMoveDir = e_Direction::eLeft;
		if (m_backgroundPosition.x < -constants::k_backgroundTileWidth)
		{
			m_backgroundPosition.x = 0;
		}
	} else if (m_player.GetCurrentDirection() == e_Direction::eLeft && m_player.GetMoveDirectionLimit() != e_Direction::eLeft)
	{
		m_backgroundPosition.x += 1.f;
		m_backgroundMoveDir = e_Direction::eRight;
		if (m_backgroundPosition.x > constants::k_backgroundTileWidth)
		{
			m_backgroundPosition.x = 0;
		}
	}

	// Reset the clock
	m_gameClock = clock();
}

void Game::Render()
{
	Graphics::GetInstance().ClearScreen();

	Graphics::GetInstance().DrawTexture("Background", { m_backgroundPosition.x - constants::k_backgroundTileWidth, 0 });
	Graphics::GetInstance().DrawTexture("Background", { m_backgroundPosition.x - 2 * constants::k_backgroundTileWidth, 0 });
	Graphics::GetInstance().DrawTexture("Background", m_backgroundPosition);
	Graphics::GetInstance().DrawTexture("Background", { m_backgroundPosition.x + constants::k_backgroundTileWidth, 0 });
	Graphics::GetInstance().DrawTexture("Background", { m_backgroundPosition.x + 2 * constants::k_backgroundTileWidth, 0 });

	const auto playerXOffset = m_player.GetPosition().x;

	DrawTiles(playerXOffset);

	for (auto& coin : m_coins)
	{
		if (coin.GetIsVisible())
		{
			coin.Render(playerXOffset);
		}
	}

	for (auto& slime : m_slimes)
	{
		slime.Render(playerXOffset);
	}

	for (auto& snail : m_snails)
	{
		snail.Render(playerXOffset);
	}

	m_player.Render();
}

void Game::CreateSprite(const std::string& spriteSheetIdentifier)
{
	if (!Graphics::GetInstance().CreateSprite(spriteSheetIdentifier, m_currentSprite))
	{
		HAPI.Close();
	}
	m_currentSprite++;
}

float Game::DeltaTime() const
{
	const clock_t programTickCount = clock() - m_gameClock;

	const float ticksToMilliseconds = 1000.f / CLOCKS_PER_SEC;

	return programTickCount * ticksToMilliseconds;
}

bool Game::GetKey(const e_KeyCode keyCode) const
{
	return m_keyboardData.scanCode[static_cast<int>(keyCode)] ? true : false;
}

void Game::HandleKeyBoardInput()
{
	if (GetKey(e_KeyCode::SPACE))
	{
		if (m_player.GetCurrentPlayerState() != e_PlayerState::eJumping)
		{
			m_player.SetShouldJump(true);
		}
	}

	e_Direction playerMoveDir = e_Direction::eNone;
	if (GetKey(e_KeyCode::A) || GetKey(e_KeyCode::LEFT))
	{
		playerMoveDir = e_Direction::eLeft;
	} else if (GetKey(e_KeyCode::D) || GetKey(e_KeyCode::RIGHT))
	{
		playerMoveDir = e_Direction::eRight;
	}

	m_player.SetDirection(playerMoveDir);
}

void Game::HandleControllerInput()
{
	//Player One Controls 
	Vector2 leftStickVector{ static_cast<float>(m_controllerData.analogueButtons[2]), static_cast<float>(m_controllerData.analogueButtons[3]) };
	if (leftStickVector.Magnitude() > constants::k_leftThumbDeadzone)
	{
		leftStickVector.Normalised();
		if (leftStickVector.x > 0)
		{
			m_player.SetDirection(e_Direction::eRight);
		} else if (leftStickVector.x < 0)
		{
			m_player.SetDirection(e_Direction::eLeft);
		} else
		{
			m_player.SetDirection(e_Direction::eNone);
		}
	}
}

bool Game::Initialise()
{
	if (!Graphics::GetInstance().CreateTexture("Data/PlatformerBackground.tga", "Background"))
	{
		return false;
	}
	if (!Graphics::GetInstance().CreateSpriteSheet("Data/GameSpriteSheet.tga"))
	{
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
	CreateSprite("Coin_1");
	CreateSprite("Coin_2");
	CreateSprite("Coin_3");
	CreateSprite("Coin_4");
	CreateSprite("Coin_5");
	CreateSprite("Coin_6");
	CreateSprite("Coin_7");
	CreateSprite("Coin_8");
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
	CreateSprite("Snail_Snail_Shell_Hit");
	CreateSprite("Snail_Shell");
	CreateSprite("Snail_Shell_Crack_1");
	CreateSprite("Snail_Shell_Crack_2");
	CreateSprite("Snail_Shell_Crack_3");
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
	if (!LoadLevel())
	{
		return false;
	}

	for (auto& enemyLocation : m_entityLocations)
	{
		switch (enemyLocation.first)
		{
		case e_EntityType::eCoin:
			m_coins.emplace_back(enemyLocation.second, true);
			break;
		case e_EntityType::eSlime:
			m_slimes.emplace_back(enemyLocation.second, enemyLocation.second.y == 768.f ? true : false);
			break;
		case e_EntityType::eSnail:
			m_snails.emplace_back(enemyLocation.second);
			break;
		default: break;
		}
	}

	m_snails.emplace_back(Vector2(1400, 768));

	m_player.SetPosition(Vector2::CENTRE);

	return true;
}

bool Game::LoadLevel()
{
	std::ifstream file("Data/Level1.csv");
	if (!file.is_open())
	{
		return false;
	}
	while (!file.eof())
	{
		for (int r = 0; r < constants::k_maxTilesVertical; ++r)
		{
			std::vector<Tile> row;
			std::string line;
			std::getline(file, line);
			if (!file.good())
			{
				break;
			}
			std::stringstream iss(line);
			for (int c = 0; c < constants::k_maxTilesHorizontal * 11; ++c)
			{
				std::string val;
				std::getline(iss, val, ',');
				if (!iss.good())
				{
					break;
				}

				const auto tileString = atoi(val.c_str());

				const Vector2 tilePosition{
					static_cast<float>(c * constants::k_spriteSheetCellWidth - 10 * constants::k_spriteSheetCellWidth),
					static_cast<float>(r * constants::k_spriteSheetCellWidth)
				};

				const auto tileType = static_cast<e_TileType>(tileString);

				if (tileType == e_TileType::eSlime ||
					tileType == e_TileType::eCoin ||
					tileType == e_TileType::eSnail)
				{
					m_entityLocations.push_back({ static_cast<const e_EntityType>(tileString), tilePosition });
					row.emplace_back(e_TileType::eAir, tilePosition, false);
				} else
				{
					bool canCollide = true;

					if (tileType == e_TileType::eAir || tileType == e_TileType::ePlant ||
						tileType == e_TileType::eRock || tileType == e_TileType::eBush ||
						tileType == e_TileType::eMushroom1 || tileType == e_TileType::eMushroom2 ||
						tileType == e_TileType::eRightArrow)
					{
						canCollide = false;
					}

					row.emplace_back(tileType, tilePosition, canCollide);
				}
			}
			m_levelData.emplace_back(row);
		}
	}
	return true;
}

void Game::CheckPlayerLevelCollisions(const CollisionBoxes playerCollisionBoxes)
{
	/* TOP COLLISIONS */
	if (m_player.GetPosition().y > static_cast<float>(constants::k_spriteSheetCellWidth) / 2.f)
	{
		const int headX = ((static_cast<int>(playerCollisionBoxes.m_topCollisionBox.TOP_LEFT.x)) /
			constants::k_spriteSheetCellWidth) + constants::k_maxTilesHorizontal / 2;

		const int headY = static_cast<int>(playerCollisionBoxes.m_topCollisionBox.TOP_LEFT.y) / constants::k_spriteSheetCellWidth;

		if (m_levelData[headY][headX].m_canCollide)
		{
			m_player.SetVelocity({ m_player.GetVelocity().x });

			Tile& currentTile = m_levelData[headY][headX];
			switch (currentTile.m_type)
			{
			case e_TileType::eCrateBlock:
			case e_TileType::eCoinBlock:
				currentTile.m_type = e_TileType::eAir;
				currentTile.m_canCollide = false;
				break;
			case e_TileType::eBoxedCoinBlock:
				currentTile.m_type = e_TileType::eCoinBlock;
			case e_TileType::eItemBlock:
				currentTile.m_type = e_TileType::eBrickBlock;
				break;
			default:;
			}
		}
	}

	/* LEFT AND RIGHT COLLISIONS */

	bool hasCollided = false;

	// LEFT
	int playerXTile = ((static_cast<int>(playerCollisionBoxes.m_leftCollisionBox.TOP_LEFT.x)) / constants::k_spriteSheetCellWidth) +
		constants::k_maxTilesHorizontal / 2;
	int playerYTile = ((static_cast<int>(playerCollisionBoxes.m_leftCollisionBox.TOP_LEFT.y)) / constants::k_spriteSheetCellWidth);

	if (m_levelData[playerYTile][playerXTile].m_canCollide)
	{
		// Work out the amount of overlap in the X direction
		const float xOverlap = static_cast<float>(playerXTile * constants::k_spriteSheetCellWidth) - playerCollisionBoxes.m_leftCollisionBox.TOP_LEFT.x -
			static_cast<float>(constants::k_screenWidth / 2.f);

		if (abs(xOverlap) > 32.f)
		{
			m_player.SetMoveDirectionLimit(e_Direction::eLeft);
			hasCollided = true;
		}
	}

	// RIGHT
	playerXTile = ((static_cast<int>(playerCollisionBoxes.m_rightCollisionBox.BOTTOM_RIGHT.x)) / constants::k_spriteSheetCellWidth) +
		constants::k_maxTilesHorizontal / 2;
	playerYTile = ((static_cast<int>(playerCollisionBoxes.m_rightCollisionBox.TOP_LEFT.y)) / constants::k_spriteSheetCellWidth);

	if (m_levelData[playerYTile][playerXTile].m_canCollide)
	{
		// Work out the amount of overlap in the X direction
		const float xOverlap = static_cast<float>(playerXTile * constants::k_spriteSheetCellWidth) - playerCollisionBoxes.m_rightCollisionBox.BOTTOM_RIGHT.x -
			static_cast<float>(constants::k_screenWidth / 2.f);

		if (abs(xOverlap) > 8.f)
		{
			m_player.SetMoveDirectionLimit(e_Direction::eRight);
			hasCollided = true;
		}
	}

	if (!hasCollided)
	{
		m_player.SetMoveDirectionLimit(e_Direction::eNone);
	}


	/* BOTTOM COLLISIONS */
	const int feetX = ((static_cast<int>(playerCollisionBoxes.m_bottomCollisionBox.TOP_LEFT.x)) /
		constants::k_spriteSheetCellWidth) + constants::k_maxTilesHorizontal / 2;
	const int feetY = static_cast<int>(playerCollisionBoxes.m_bottomCollisionBox.TOP_LEFT.y) / constants::k_spriteSheetCellWidth;

	if (m_levelData[feetY][feetX].m_canCollide)
	{
		// Work out the amount of overlap in the Y direction
		const float yOverlap = static_cast<float>(feetY * constants::k_spriteSheetCellWidth) - playerCollisionBoxes.m_bottomCollisionBox.TOP_LEFT.y;
		if (abs(yOverlap) > 16.f)
		{
			m_player.SetPosition({ m_player.GetPosition().x, static_cast<float>(feetY - 1) * constants::k_spriteSheetCellWidth });
		}
		m_player.SetPlayerState(e_PlayerState::eWalking);
	} else
	{
		m_player.SetPlayerState(e_PlayerState::eJumping);
	}
}

void Game::CheckEnemyLevelCollisions(Enemy* enemy)
{
	const auto enemyPos = enemy->GetPosition();

	const int enemyXTile = ((static_cast<int>(enemyPos.x)) / constants::k_spriteSheetCellWidth) + constants::k_maxTilesHorizontal / 2;

	const int enemyYTile = static_cast<int>(enemyPos.y) / constants::k_spriteSheetCellWidth;

	if (enemyYTile > 0 && enemyYTile < constants::k_maxTilesVertical)
	{
		// Stop falling if there is a walkable block below
		if (m_levelData[enemyYTile + 1][enemyXTile].m_canCollide)
		{
			if (m_levelData[enemyYTile + 1][enemyXTile].m_type == e_TileType::eSpikes)
			{
				enemy->Squash();
			}
			enemy->SetIsFalling(false);
		}

		// Check if there is a block to the left
		if (m_levelData[enemyYTile][enemyXTile].m_canCollide && enemy->GetCurrentDirection() == e_Direction::eLeft)
		{
			enemy->SetDirection(e_Direction::eRight);
		}

		// Check if there is a block to the right
		if (m_levelData[enemyYTile][enemyXTile + 1].m_canCollide && enemy->GetCurrentDirection() == e_Direction::eRight)
		{
			enemy->SetDirection(e_Direction::eLeft);
		}

		// If the enemy can stay on platforms...
		if (m_levelData[enemyYTile + 1][enemyXTile].m_type == e_TileType::eAir && enemy->GetCurrentDirection() == e_Direction::eLeft)
		{
			if (enemy->CanAvoidEdges())
			{
				enemy->SetDirection(e_Direction::eRight);
			} else
			{
				enemy->SetIsFalling(true);
			}
		}
		if (m_levelData[enemyYTile + 1][enemyXTile + 1].m_type == e_TileType::eAir && enemy->GetCurrentDirection() == e_Direction::eRight)
		{
			if (enemy->CanAvoidEdges())
			{
				enemy->SetDirection(e_Direction::eLeft);
			} else
			{
				enemy->SetIsFalling(true);
			}
		}
	}
}

void Game::DrawTiles(const float playerXOffset)
{
	for (auto& row : m_levelData)
	{
		for (const auto& currentTile : row)
		{
			if (currentTile.m_type != e_TileType::eAir)
			{
				const Vector2 tilePos = {
					currentTile.m_position.x - playerXOffset + static_cast<float>(constants::k_screenWidth) / 2.f,
					currentTile.m_position.y
				};
				if (tilePos.x > -constants::k_spriteSheetCellWidth && tilePos.x <= constants::k_screenWidth)
				{
					std::string spriteIdentifier;
					switch (currentTile.m_type)
					{
					case e_TileType::eAir:
						spriteIdentifier = "Air";
						break;
					case e_TileType::eDirt:
						spriteIdentifier = "Dirt";
						break;
					case e_TileType::eGrassLeft:
						spriteIdentifier = "Grass_Left";
						break;
					case e_TileType::eGrassCentre:
						spriteIdentifier = "Grass_Centre";
						break;
					case e_TileType::eGrassRight:
						spriteIdentifier = "Grass_Right";
						break;
					case e_TileType::eStoneTop:
						spriteIdentifier = "Stone_Top";
						break;
					case e_TileType::eStoneCentre:
						spriteIdentifier = "Stone_Centre";
						break;
					case e_TileType::eStoneLeft:
						spriteIdentifier = "Stone_Left";
						break;
					case e_TileType::eStoneRight:
						spriteIdentifier = "Stone_Right";
						break;
					case e_TileType::eFlag:
						spriteIdentifier = "Flag_Up_1";
						break;
					case e_TileType::eCoinBlock:
						spriteIdentifier = "Block_Coin";
						break;
					case e_TileType::eBoxedCoinBlock:
						spriteIdentifier = "Block_Boxed_Coin";
						break;
					case e_TileType::eCrateBlock:
						spriteIdentifier = "Block_Crate";
						break;
					case e_TileType::eItemBlock:
						spriteIdentifier = "Block_Item";
						break;
					case e_TileType::eBrickBlock:
						spriteIdentifier = "Block_Brick";
						break;
					case e_TileType::eBush:
						spriteIdentifier = "Bush";
						break;
					case e_TileType::eOpenDoorMid:
						spriteIdentifier = "Door_Open_Mid";
						break;
					case e_TileType::eOpenDoorTop:
						spriteIdentifier = "Door_Open_Top";
						break;
					case e_TileType::ePlant:
						spriteIdentifier = "Plant";
						break;
					case e_TileType::eMushroom1:
						spriteIdentifier = "Mushroom1";
						break;
					case e_TileType::eMushroom2:
						spriteIdentifier = "Mushroom2";
						break;
					case e_TileType::eRock:
						spriteIdentifier = "Rock";
						break;
					case e_TileType::eSpikes:
						spriteIdentifier = "Spikes";
						break;
					case e_TileType::eFlagPole:
						spriteIdentifier = "Flag_Pole";
						break;
					case e_TileType::eRightArrow:
						spriteIdentifier = "Arrow_Sign";
						break;
					default:;
					}
					Graphics::GetInstance().DrawSprite(spriteIdentifier, tilePos);
				}
			}
		}
	}
}
