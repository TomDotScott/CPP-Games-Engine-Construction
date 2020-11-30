#include "TileManager.h"
#include <fstream>
#include <sstream>
#include "Player.h"
#include "Enemies/Enemy.h"
#include "../Graphics/Graphics.h"

bool TileManager::LoadLevel(const std::string& filename)
{
	std::cout << filename << std::endl;
	std::ifstream file(filename);
	if (!file.is_open())
	{
		HAPI.UserMessage("Couldn't Open the File: " + filename + "\nCheck the location and try again", "An error occured");
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
	file.close();
	return true;
}

void TileManager::RenderTiles(const float playerOffset)
{
	for (auto& row : m_levelData)
	{
		for (const auto& currentTile : row)
		{
			if (currentTile.m_type != e_TileType::eAir)
			{
				const Vector2 tilePos = {
					currentTile.m_position.x - playerOffset + static_cast<float>(constants::k_screenWidth) / 2.f,
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

void TileManager::CheckPlayerLevelCollisions(Player& player)
{
	const CollisionBoxes playerCollisionBoxes = player.GetCurrentCollisionBoxes();
	/* TOP COLLISIONS */
	if (player.GetPosition().y > static_cast<float>(constants::k_spriteSheetCellWidth) / 2.f)
	{
		const int headX = ((static_cast<int>(playerCollisionBoxes.m_topCollisionBox.TOP_LEFT.x)) /
			constants::k_spriteSheetCellWidth) + constants::k_maxTilesHorizontal / 2;

		const int headY = static_cast<int>(playerCollisionBoxes.m_topCollisionBox.TOP_LEFT.y) / constants::k_spriteSheetCellWidth;

		if (m_levelData[headY][headX].m_canCollide)
		{
			player.SetVelocity({ player.GetVelocity().x });

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
			player.SetMoveDirectionLimit(e_Direction::eLeft);
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
			player.SetMoveDirectionLimit(e_Direction::eRight);
			hasCollided = true;
		}
	}

	if (!hasCollided)
	{
		player.SetMoveDirectionLimit(e_Direction::eNone);
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
			player.SetPosition({ player.GetPosition().x, static_cast<float>(feetY - 1) * constants::k_spriteSheetCellWidth });
		}
		player.SetPlayerState(e_PlayerState::eWalking);
	} else
	{
		player.SetPlayerState(e_PlayerState::eJumping);
	}
}

void TileManager::CheckEnemyLevelCollisions(Enemy& enemy)
{
	const auto enemyPos = enemy.GetPosition();

	const int enemyXTile = ((static_cast<int>(enemyPos.x)) / constants::k_spriteSheetCellWidth) + constants::k_maxTilesHorizontal / 2;

	const int enemyYTile = static_cast<int>(enemyPos.y) / constants::k_spriteSheetCellWidth;

	if (enemyYTile > 0 && enemyYTile < constants::k_maxTilesVertical)
	{
		// Stop falling if there is a walkable block below
		if (m_levelData[enemyYTile + 1][enemyXTile].m_canCollide)
		{
			if (m_levelData[enemyYTile + 1][enemyXTile].m_type == e_TileType::eSpikes)
			{
				enemy.Squash();
			}
			enemy.SetIsFalling(false);
		}

		// Check if there is a block to the left
		if (m_levelData[enemyYTile][enemyXTile].m_canCollide && enemy.GetCurrentDirection() == e_Direction::eLeft)
		{
			enemy.SetDirection(e_Direction::eRight);
		}

		// Check if there is a block to the right
		if (m_levelData[enemyYTile][enemyXTile + 1].m_canCollide && enemy.GetCurrentDirection() == e_Direction::eRight)
		{
			enemy.SetDirection(e_Direction::eLeft);
		}

		// If the enemy can stay on platforms...
		if (m_levelData[enemyYTile + 1][enemyXTile].m_type == e_TileType::eAir && enemy.GetCurrentDirection() == e_Direction::eLeft)
		{
			if (enemy.CanAvoidEdges())
			{
				enemy.SetDirection(e_Direction::eRight);
			} else
			{
				enemy.SetIsFalling(true);
			}
		}
		if (m_levelData[enemyYTile + 1][enemyXTile + 1].m_type == e_TileType::eAir && enemy.GetCurrentDirection() == e_Direction::eRight)
		{
			if (enemy.CanAvoidEdges())
			{
				enemy.SetDirection(e_Direction::eLeft);
			} else
			{
				enemy.SetIsFalling(true);
			}
		}
	}
}

entity_locations TileManager::GetEntityLocations() const
{
	return m_entityLocations;
}
