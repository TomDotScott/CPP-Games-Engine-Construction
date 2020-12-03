#pragma once
#include <vector>
#include "../Utilities/Vector.h"
#include "Entities/Enemies/Enemy.h"
#include "Entities/Entity.h"
#include "Entities/Player.h"
struct Tile;
using entity_locations = std::vector<std::pair<e_EntityType, Vector2>>;

class TileManager
{
public:
	bool LoadLevel(const std::string& filename);
	void RenderTiles(float playerOffset);
	void CheckPlayerLevelCollisions(Player& player);
	void CheckEnemyLevelCollisions(Enemy& enemy);
	entity_locations GetEntityLocations() const;
	
private:
	std::vector<std::vector<Tile>> m_levelData;
	entity_locations m_entityLocations;
};

enum class e_TileType
{
	eAir = -1, eDirt = 0, eGrassLeft = 1, eGrassCentre = 2,
	eGrassRight = 3, eStoneTop = 4, eStoneCentre = 5,
	eStoneLeft = 6, eStoneRight = 7, eFlag = 38,
	eCoinBlock = 47, eBoxedCoinBlock = 48, eCrateBlock = 49,
	eItemBlock = 50, eBrickBlock = 51, eBush = 52, eOpenDoorMid = 55,
	eOpenDoorTop = 56, ePlant = 57, eMushroom1 = 60, eMushroom2 = 61,
	eRock = 62, eSpikes = 63, eFlagPole = 64, eSlime = 65, eCoin = 66,
	eSnail = 68, eRightArrow = 69
};

struct Tile
{
	Tile(const e_TileType type, const Vector2 position, const bool canCollide) :
		m_type(type),
		m_position(position),
		m_canCollide(canCollide)
	{
	}

	e_TileType m_type;
	Vector2 m_position;
	bool m_canCollide;
};

