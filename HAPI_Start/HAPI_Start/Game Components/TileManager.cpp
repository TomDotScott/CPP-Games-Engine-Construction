#include "TileManager.h"
#include <fstream>
#include <sstream>

#include "../Audio/SoundManager.h"
#include "../Graphics/TextureManager.h"

TileManager::TileManager() :
	m_alienCollisionData() {}

bool TileManager::LoadLevel(const std::string& filename)
{
	// Clear the level data if it exists
	m_levelData.clear();

	// Clear the entity locations
	m_entityLocations.clear();

	std::ifstream file(filename);
	if (!file.is_open())
	{
		HAPI.UserMessage("Couldn't Open the File: " + filename + "\nCheck the location and try again",
		                 "An error occurred");
		return false;
	}

	while (!file.eof())
	{
		for (int r = 0; r < constants::k_maxTilesVertical; ++r)
		{
			std::vector<Tile> row;
			std::string       line;
			std::getline(file, line);
			if (!file.good())
			{
				break;
			}
			std::stringstream iss(line);
			for (int c = 0; c < constants::k_maxTilesHorizontal * 30; ++c)
			{
				std::string val;
				std::getline(iss, val, ',');
				if (!iss.good())
				{
					break;
				}

				const auto tileIntFromCSV = atoi(val.c_str());

				const Vector2 tilePosition{
					static_cast<float>(c * constants::k_spriteSheetCellSize - 10 * constants::k_spriteSheetCellSize),
					static_cast<float>(r * constants::k_spriteSheetCellSize)
				};

				const auto tileType = static_cast<eTileType>(tileIntFromCSV);

				switch (tileType)
				{
					case eTileType::e_Dirt:
					case eTileType::e_GrassLeft:
					case eTileType::e_GrassCentre:
					case eTileType::e_GrassRight:
					case eTileType::e_GrassCliffLeft:
					case eTileType::e_GrassCliffRight:
					case eTileType::e_StoneTop:
					case eTileType::e_Stone:
					case eTileType::e_StoneLeft:
					case eTileType::e_StoneCentre:
					case eTileType::e_StoneRight:
					case eTileType::e_FlagPole:
					case eTileType::e_CoinBlock:
					case eTileType::e_BoxedCoinBlock:
					case eTileType::e_CrateBlock:
					case eTileType::e_ItemBlock:
					case eTileType::e_BrickBlock:
					case eTileType::e_ClosedDoorMid:
					case eTileType::e_ClosedDoorTop:
					case eTileType::e_DestructibleStone:
					case eTileType::e_Bridge:
					case eTileType::e_OpenDoorMid:
					case eTileType::e_OpenDoorTop:
					case eTileType::e_Spikes:
						row.emplace_back(tileType, tilePosition, true);
						break;

					case eTileType::e_Air:
					case eTileType::e_Bush:
					case eTileType::e_RightArrow:
					case eTileType::e_Plant:
					case eTileType::e_Mushroom1:
					case eTileType::e_Mushroom2:
					case eTileType::e_Rock:
						row.emplace_back(tileType, tilePosition, false);
						break;

					case eTileType::e_Boss:
					case eTileType::e_Lever:
					case eTileType::e_Flag:
					case eTileType::e_Slime:
					case eTileType::e_Coin:
					case eTileType::e_Snail:
						m_entityLocations.emplace_back(static_cast<const eEntityType>(tileIntFromCSV), tilePosition);
						row.emplace_back(eTileType::e_Air, tilePosition, false);
						break;
				}
			}
			m_levelData.emplace_back(row);
		}
	}
	file.close();

	return true;
}

void TileManager::RenderTiles(TextureManager& textureManager, const float playerOffset)
{
	for (auto& row : m_levelData)
	{
		for (const auto& currentTile : row)
		{
			if (currentTile.m_type != eTileType::e_Air)
			{
				const Vector2 tilePos = {
					currentTile.m_position.x - playerOffset + static_cast<float>(constants::k_screenWidth) / 2.f,
					currentTile.m_position.y
				};

				if (tilePos.x > -constants::k_spriteSheetCellSize && tilePos.x <= constants::k_screenWidth)
				{
					std::string spriteIdentifier = "EmptyString";
					switch (currentTile.m_type)
					{
						case eTileType::e_Air:
							spriteIdentifier = "Air";
							break;
						case eTileType::e_Dirt:
							spriteIdentifier = "Dirt";
							break;
						case eTileType::e_GrassLeft:
							spriteIdentifier = "Grass_Left";
							break;
						case eTileType::e_GrassCentre:
							spriteIdentifier = "Grass_Centre";
							break;
						case eTileType::e_GrassRight:
							spriteIdentifier = "Grass_Right";
							break;
						case eTileType::e_StoneTop:
							spriteIdentifier = "Stone_Top";
							break;
						case eTileType::e_DestructibleStone:
						case eTileType::e_Stone:
							spriteIdentifier = "Stone";
							break;
						case eTileType::e_StoneLeft:
							spriteIdentifier = "Stone_Left";
							break;
						case eTileType::e_StoneRight:
							spriteIdentifier = "Stone_Right";
							break;
						case eTileType::e_Flag:
							spriteIdentifier = "Flag_Up_1";
							break;
						case eTileType::e_CoinBlock:
							spriteIdentifier = "Block_Coin";
							break;
						case eTileType::e_BoxedCoinBlock:
							spriteIdentifier = "Block_Boxed_Coin";
							break;
						case eTileType::e_CrateBlock:
							spriteIdentifier = "Block_Crate";
							break;
						case eTileType::e_ItemBlock:
							spriteIdentifier = "Block_Item";
							break;
						case eTileType::e_BrickBlock:
							spriteIdentifier = "Block_Brick";
							break;
						case eTileType::e_Bush:
							spriteIdentifier = "Bush";
							break;
						case eTileType::e_OpenDoorMid:
							spriteIdentifier = "Door_Open_Mid";
							break;
						case eTileType::e_OpenDoorTop:
							spriteIdentifier = "Door_Open_Top";
							break;
						case eTileType::e_ClosedDoorMid:
							spriteIdentifier = "Door_Closed_Mid";
							break;
						case eTileType::e_ClosedDoorTop:
							spriteIdentifier = "Door_Closed_Top";
							break;
						case eTileType::e_Plant:
							spriteIdentifier = "Plant";
							break;
						case eTileType::e_Mushroom1:
							spriteIdentifier = "Mushroom1";
							break;
						case eTileType::e_Mushroom2:
							spriteIdentifier = "Mushroom2";
							break;
						case eTileType::e_Rock:
							spriteIdentifier = "Rock";
							break;
						case eTileType::e_Spikes:
							spriteIdentifier = "Spikes";
							break;
						case eTileType::e_FlagPole:
							spriteIdentifier = "Flag_Pole";
							break;
						case eTileType::e_RightArrow:
							spriteIdentifier = "Arrow_Sign";
							break;
						case eTileType::e_GrassCliffLeft:
							spriteIdentifier = "Grass_Cliff_Left";
							break;
						case eTileType::e_GrassCliffRight:
							spriteIdentifier = "Grass_Cliff_Right";
							break;
						case eTileType::e_Bridge:
							spriteIdentifier = "Bridge";
							break;
						case eTileType::e_StoneCentre:
							spriteIdentifier = "Stone_Centre";
							break;
						case eTileType::e_Slime:
						case eTileType::e_Coin:
						case eTileType::e_Snail:
						case eTileType::e_Boss:
						case eTileType::e_Lever:
							break;
					}
					textureManager.DrawSprite(spriteIdentifier, tilePos);
				}
			}
		}
	}
}

CollisionData& TileManager::CheckAlienLevelCollisions(Alien& alien)
{
	m_alienCollisionData.Reset();

	const CollisionBoxes playerCollisionBoxes = alien.GetCurrentCollisionBoxes();

	m_alienCollisionData.m_headCollision = CheckAlienHeadCollisions(alien, playerCollisionBoxes);

	m_alienCollisionData.m_leftCollision = CheckAlienLeftCollisions(alien, playerCollisionBoxes);

	m_alienCollisionData.m_rightCollision = CheckAlienRightCollisions(alien, playerCollisionBoxes);

	m_alienCollisionData.m_bottomCollision = CheckAlienBottomCollisions(alien, playerCollisionBoxes);

	for (auto& fireball : alien.GetFireBallPool())
	{
		CheckFireballLevelCollisions(fireball);
	}

	return m_alienCollisionData;
}

bool TileManager::IsBossOnFloor(Alien& boss)
{
	if (CheckAlienBottomCollisions(boss, boss.GetCurrentCollisionBoxes()))
	{
		return true;
	}

	return false;
}

void TileManager::OnLeverPulled()
{
	for (auto& row : m_levelData)
	{
		for (auto& currentTile : row)
		{
			if (currentTile.m_type == eTileType::e_DestructibleStone || currentTile.m_type == eTileType::e_Bridge)
			{
				currentTile.m_type       = eTileType::e_Air;
				currentTile.m_canCollide = false;
			}
		}
	}
}

void TileManager::CheckEnemyLevelCollisions(Enemy& enemy)
{
	const auto enemyPos = enemy.GetPosition();

	const int enemyX = static_cast<int>(enemyPos.x) / constants::k_spriteSheetCellSize +
	                            constants::k_maxTilesHorizontal / 2;

	const int enemyY = static_cast<int>(enemyPos.y) / constants::k_spriteSheetCellSize;
	
	if (enemyY > 0 && enemyY + 1 < constants::k_maxTilesVertical && enemyX > 0)
	{
		const auto enemyXTile = static_cast<uint64_t>(enemyX);
		const auto enemyYTile = static_cast<uint64_t>(enemyY);
		
		// Stop falling if there is a walkable block below
		if (m_levelData[enemyYTile + 1][enemyXTile].m_canCollide)
		{
			if (m_levelData[enemyYTile + 1][enemyXTile].m_type == eTileType::e_Spikes)
			{
				enemy.Squash();
			}
			enemy.SetIsFalling(false);
		}

		// Check if there is a block to the left
		if (m_levelData[enemyYTile][enemyXTile].m_canCollide && enemy.GetCurrentDirection() == eDirection::e_Left)
		{
			enemy.SetDirection(eDirection::e_Right);
		}

		// Check if there is a block to the right
		if (m_levelData[enemyYTile][enemyXTile + 1].m_canCollide && enemy.GetCurrentDirection() == eDirection::e_Right)
		{
			enemy.SetDirection(eDirection::e_Left);
		}

		// If the enemy can stay on platforms...
		if (m_levelData[enemyYTile + 1][enemyXTile].m_type == eTileType::e_Air && enemy.GetCurrentDirection() ==
		    eDirection::e_Left)
		{
			if (enemy.CanAvoidEdges())
			{
				enemy.SetDirection(eDirection::e_Right);
			}
			else
			{
				enemy.SetIsFalling(true);
			}
		}
		if (m_levelData[enemyYTile + 1][enemyXTile + 1].m_type == eTileType::e_Air && enemy.GetCurrentDirection() ==
		    eDirection::e_Right)
		{
			if (enemy.CanAvoidEdges())
			{
				enemy.SetDirection(eDirection::e_Left);
			}
			else
			{
				enemy.SetIsFalling(true);
			}
		}
	}
}

void TileManager::CheckFireballLevelCollisions(Fireball& fireball)
{
	// COLLISIONS WITH THE GROUND
	auto fireballBottom = fireball.GetCurrentCollisionBoxes().m_bottomCollisionBox;

	int fireballX = ((static_cast<int>(fireballBottom.TOP_LEFT.x)) / constants::k_spriteSheetCellSize) +
	                constants::k_maxTilesHorizontal / 2;

	int fireballY = static_cast<int>(fireballBottom.BOTTOM_RIGHT.y) / constants::k_spriteSheetCellSize;


	if (fireballX >= 0 && fireballX <= static_cast<int>(m_levelData[0].size()) && fireballY < 15 && fireballY >= 0)
	{
		auto& groundTile = m_levelData[fireballY][fireballX];

		const auto groundTileBoxes = groundTile.m_tileCollisionBox;

		if (fireballBottom.Translate({static_cast<float>(constants::k_maxTilesHorizontal) / 2.f, 0}).
		                   Overlapping(groundTileBoxes)
		    && groundTile.m_canCollide)
		{
			fireball.Bounce();
		}
	}

	// COLLISIONS TO THE RIGHT
	const auto& fireballRight = fireball.GetCurrentCollisionBoxes().m_rightCollisionBox;

	fireballX = ((static_cast<int>(fireballRight.BOTTOM_RIGHT.x)) / constants::k_spriteSheetCellSize) +
	            constants::k_maxTilesHorizontal / 2;

	fireballY = static_cast<int>(fireballRight.BOTTOM_RIGHT.y) / constants::k_spriteSheetCellSize;


	if (fireballX >= 0 && fireballX <= static_cast<int>(m_levelData[0].size()) && fireballY < 15 && fireballY >= 0)
	{
		auto& sideTile = m_levelData[fireballY][static_cast<uint64_t>(fireballX) + 1];

		const auto sideTileBounds = CollisionBox({sideTile.m_position},
		                                         {
			                                         sideTile.m_position.x + constants::k_spriteSheetCellSize,
			                                         sideTile.m_position.y + constants::k_spriteSheetCellSize
		                                         });

		if (fireballBottom.Translate({static_cast<float>(constants::k_maxTilesHorizontal) / 2.f, 0}).
		                   Overlapping(sideTileBounds)
		    && sideTile.m_canCollide)
		{
			fireball.Explode();
		}
	}
}

std::vector<std::pair<eEntityType, Vector2>>& TileManager::GetEntityLocations()
{
	return m_entityLocations;
}

Tile* TileManager::CheckAlienHeadCollisions(Alien& alien, const CollisionBoxes& alienCollisionBoxes)
{
	const CollisionBox& playerGlobalBox = alienCollisionBoxes.m_globalBounds;

	// Find the grid position of the global bounds...
	const int globalBoundsX = (static_cast<int>(playerGlobalBox.TOP_LEFT.x) / constants::k_spriteSheetCellSize) +
	                          constants::k_maxTilesHorizontal / 2;
	const int globalBoundsY = (static_cast<int>(playerGlobalBox.TOP_LEFT.y) / constants::k_spriteSheetCellSize);

	// check the player is on screen
	if (globalBoundsX > 0 && globalBoundsX < static_cast<int>(m_levelData[0].size()) && globalBoundsY > 0 &&
	    globalBoundsY < constants::k_maxTilesVertical)
	{
		auto& currentTile = m_levelData[globalBoundsY][globalBoundsX];
		if (currentTile.m_canCollide && playerGlobalBox.Overlapping(currentTile.m_tileCollisionBox))
		{
			const CollisionBox& playerTopCollisionBox = alienCollisionBoxes.m_topCollisionBox;
			if (playerTopCollisionBox.Overlapping(currentTile.m_tileCollisionBox))
			{
				return &currentTile;
			}
		}
	}
	return nullptr;
}

Tile* TileManager::CheckAlienLeftCollisions(Alien& alien, const CollisionBoxes& alienCollisionBoxes)
{
	const CollisionBox& playerLeftCollisionBox = alienCollisionBoxes.m_leftCollisionBox;

	const int playerX = (static_cast<int>(playerLeftCollisionBox.TOP_LEFT.x) / constants::k_spriteSheetCellSize) +
	                    constants::k_maxTilesHorizontal / 2;
	const int playerY = static_cast<int>(playerLeftCollisionBox.TOP_LEFT.y) / constants::k_spriteSheetCellSize;

	if (playerX > 0 && playerX < static_cast<int>(m_levelData[0].size()) && playerY > 0 && playerY <
	    constants::k_maxTilesVertical)
	{
		auto& currentTile = m_levelData[playerY][playerX];

		if (currentTile.m_canCollide && alienCollisionBoxes.m_globalBounds.Overlapping(currentTile.m_tileCollisionBox))
		{
			if (playerLeftCollisionBox.Overlapping(currentTile.m_tileCollisionBox))
			{
				return &currentTile;
			}
		}
	}

	return nullptr;
}

Tile* TileManager::CheckAlienRightCollisions(Alien& alien, const CollisionBoxes& alienCollisionBoxes)
{
	const CollisionBox& playerRightCollisionBox = alienCollisionBoxes.m_rightCollisionBox;

	const int playerX = (static_cast<int>(playerRightCollisionBox.BOTTOM_RIGHT.x) / constants::k_spriteSheetCellSize) +
	                    constants::k_maxTilesHorizontal / 2;
	const int playerY = static_cast<int>(playerRightCollisionBox.TOP_LEFT.y) / constants::k_spriteSheetCellSize;

	if (playerX > 0 && playerX < static_cast<int>(m_levelData[0].size()) && playerY > 0 && playerY <
	    constants::k_maxTilesVertical)
	{
		auto& currentTile = m_levelData[playerY][playerX];

		if (currentTile.m_canCollide && alienCollisionBoxes.m_globalBounds.Overlapping(currentTile.m_tileCollisionBox))
		{
			if (playerRightCollisionBox.Overlapping(currentTile.m_tileCollisionBox))
			{
				return &currentTile;
			}
		}
	}

	return nullptr;
}

Tile* TileManager::CheckAlienBottomCollisions(Alien& alien, const CollisionBoxes& alienCollisionBoxes)
{
	const CollisionBox& playerBottomCollisionBox = alienCollisionBoxes.m_bottomCollisionBox;

	// Find the block below the player
	const int playerFeetX = (static_cast<int>(playerBottomCollisionBox.TOP_LEFT.x) / constants::k_spriteSheetCellSize) +
	                        constants::k_maxTilesHorizontal / 2;

	const int playerFeetY = static_cast<int>(playerBottomCollisionBox.TOP_LEFT.y) / constants::k_spriteSheetCellSize;

	// Check the player is on-screen
	if (playerFeetX > 0 && playerFeetX < static_cast<int>(m_levelData[0].size()) && playerFeetY > 0 && playerFeetY <
	    constants::k_maxTilesVertical)
	{
		auto& currentTile = m_levelData[playerFeetY][playerFeetX];

		if (currentTile.m_canCollide && alienCollisionBoxes.m_globalBounds.Overlapping(currentTile.m_tileCollisionBox))
		{
			if (playerBottomCollisionBox.Overlapping(currentTile.m_tileCollisionBox))
			{
				return &currentTile;
			}
		}
	}

	return nullptr;
}
