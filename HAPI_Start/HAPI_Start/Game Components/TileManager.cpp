#include "TileManager.h"
#include <fstream>
#include <sstream>
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

				const auto tileType = static_cast<eTileType>(tileString);

				if (tileType == eTileType::e_Slime ||
					tileType == eTileType::e_Coin ||
					tileType == eTileType::e_Snail)
				{
					m_entityLocations.push_back({ static_cast<const eEntityType>(tileString), tilePosition });
					row.emplace_back(eTileType::e_Air, tilePosition, false);
				} else
				{
					bool canCollide = true;

					if (tileType == eTileType::e_Air || tileType == eTileType::e_Plant ||
						tileType == eTileType::e_Rock || tileType == eTileType::e_Bush ||
						tileType == eTileType::e_Mushroom1 || tileType == eTileType::e_Mushroom2 ||
						tileType == eTileType::e_RightArrow)
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
			if (currentTile.m_type != eTileType::e_Air)
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
					case eTileType::e_StoneCentre:
						spriteIdentifier = "Stone_Centre";
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
			case eTileType::e_CrateBlock:
			case eTileType::e_CoinBlock:
				currentTile.m_type = eTileType::e_Air;
				currentTile.m_canCollide = false;
				break;
			case eTileType::e_BoxedCoinBlock:
				currentTile.m_type = eTileType::e_CoinBlock;
			case eTileType::e_ItemBlock:
				currentTile.m_type = eTileType::e_BrickBlock;
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
			player.SetMoveDirectionLimit(eDirection::e_Left);
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
			player.SetMoveDirectionLimit(eDirection::e_Right);
			hasCollided = true;
		}
	}

	if (!hasCollided)
	{
		player.SetMoveDirectionLimit(eDirection::e_None);
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
		player.SetPlayerState(ePlayerState::e_Walking);
	} else
	{
		player.SetPlayerState(ePlayerState::e_Jumping);
	}

	for (auto& fireball : player.GetFireBallPool())
	{
		if (fireball.GetActiveState())
		{
			CheckFireballLevelCollisions(fireball);
		}
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
		if (m_levelData[enemyYTile + 1][enemyXTile].m_type == eTileType::e_Air && enemy.GetCurrentDirection() == eDirection::e_Left)
		{
			if (enemy.CanAvoidEdges())
			{
				enemy.SetDirection(eDirection::e_Right);
			} else
			{
				enemy.SetIsFalling(true);
			}
		}
		if (m_levelData[enemyYTile + 1][enemyXTile + 1].m_type == eTileType::e_Air && enemy.GetCurrentDirection() == eDirection::e_Right)
		{
			if (enemy.CanAvoidEdges())
			{
				enemy.SetDirection(eDirection::e_Left);
			} else
			{
				enemy.SetIsFalling(true);
			}
		}
	}
}

void TileManager::CheckFireballLevelCollisions(Fireball& fireball)
{
	auto fireballBottom = fireball.GetCurrentCollisionBoxes().m_bottomCollisionBox;
	
	const int fireballX = ((static_cast<int>(fireballBottom.TOP_LEFT.x)) / constants::k_spriteSheetCellWidth) + constants::k_maxTilesHorizontal / 2;

	const int fireballY = static_cast<int>(fireballBottom.BOTTOM_RIGHT.y) / constants::k_spriteSheetCellWidth;


	if (fireballX >= 0 && fireballX <= m_levelData[0].size() && fireballY <= 15 && fireballY >= 0)
	{
		const auto& groundTile = m_levelData[fireballY][fireballX];
		
		const auto groundTileBoxes = BoundsRectangle({ groundTile.m_position },
			{ groundTile.m_position.x + constants::k_spriteSheetCellWidth, groundTile.m_position.y + constants::k_spriteSheetCellWidth});

		if (fireballBottom.Translate({static_cast<float>(constants::k_maxTilesHorizontal / 2), 0 }).Overlapping(groundTileBoxes) 
			&& groundTile.m_canCollide)
		{
			fireball.Bounce();
		}
	}
}

entity_locations TileManager::GetEntityLocations() const
{
	return m_entityLocations;
}
