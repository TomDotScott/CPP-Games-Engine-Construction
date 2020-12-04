#pragma once
#include <vector>
#include "../Utilities/Vector.h"
#include "Entities/Enemies/Enemy.h"
#include "Entities/Entity.h"
#include "Entities/Player.h"
struct Tile;
using entity_locations = std::vector<std::pair<eEntityType, Vector2>>;

class TileManager
{
public:
	bool LoadLevel(const std::string& filename);
	void RenderTiles(float playerOffset);
	void CheckPlayerLevelCollisions(Player& player);
	void CheckEnemyLevelCollisions(Enemy& enemy);
	void CheckFireballLevelCollisions(Fireball& fireball);
	entity_locations GetEntityLocations() const;
	
private:
	std::vector<std::vector<Tile>> m_levelData;
	entity_locations m_entityLocations;
};

enum class eTileType
{
	e_Air = -1, e_Dirt = 0, e_GrassLeft = 1, e_GrassCentre = 2,
	e_GrassRight = 3, e_StoneTop = 4, e_StoneCentre = 5,
	e_StoneLeft = 6, e_StoneRight = 7, e_Flag = 38,
	e_CoinBlock = 47, e_BoxedCoinBlock = 48, e_CrateBlock = 49,
	e_ItemBlock = 50, e_BrickBlock = 51, e_Bush = 52, e_OpenDoorMid = 55,
	e_OpenDoorTop = 56, e_Plant = 57, e_Mushroom1 = 60, e_Mushroom2 = 61,
	e_Rock = 62, e_Spikes = 63, e_FlagPole = 64, e_Slime = 65, e_Coin = 66,
	e_Snail = 68, e_RightArrow = 69
};

struct Tile
{
	Tile(const eTileType type, const Vector2 position, const bool canCollide) :
		m_type(type),
		m_position(position),
		m_canCollide(canCollide)
	{
	}

	eTileType m_type;
	Vector2 m_position;
	bool m_canCollide;
};

