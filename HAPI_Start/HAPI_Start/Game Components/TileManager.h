#pragma once
#include <vector>
#include <queue>
#include "../Utilities/Vector.h"
#include "Entities/Enemies/Enemy.h"
#include "Entities/Entity.h"
#include "Entities/Player.h"
struct Tile;

class TileManager
{
public:
	bool LoadLevel(const std::string& filename);
	void RenderTiles(float playerOffset);
	void CheckPlayerLevelCollisions(Player& player);
	bool ShouldLoadNextLevel() const;
	void CheckEnemyLevelCollisions(Enemy& enemy);
	void CheckFireballLevelCollisions(Fireball& fireball);
	std::queue<std::pair<eEntityType, Vector2>>& GetEntityLocations();

private:
	std::vector<std::vector<Tile>> m_levelData;
	std::queue<std::pair<eEntityType, Vector2>> m_entityLocations;
	bool m_shouldLoadNextLevel{ false };
};

enum class eTileType
{
	e_Air = -1, e_Dirt = 0, e_GrassLeft = 1, e_GrassCentre = 2,
	e_GrassRight = 3, e_StoneTop = 4, e_StoneCentre = 5,
	e_StoneLeft = 6, e_StoneRight = 7, e_Flag = 38,
	e_CoinBlock = 47, e_BoxedCoinBlock = 48, e_CrateBlock = 49,
	e_ItemBlock = 50, e_BrickBlock = 51, e_Bush = 52, e_ClosedDoorMid = 53,
	e_ClosedDoorTop = 54, e_OpenDoorMid = 55, e_OpenDoorTop = 56, e_Plant = 57,
	e_Mushroom1 = 60, e_Mushroom2 = 61, e_Rock = 62, e_Spikes = 63, e_FlagPole = 64,
	e_Slime = 65, e_Coin = 66, e_Snail = 68, e_RightArrow = 69, e_GrassCliffLeft = 70,
	e_GrassCliffRight = 71
};

struct Tile
{
	Tile(const eTileType type, const Vector2 position, const bool canCollide) :
		m_type(type),
		m_position(position),
		m_canCollide(canCollide),
		m_canBeDestroyed(true)
	{
	}

	eTileType m_type;
	Vector2 m_position;
	bool m_canCollide;
	bool m_canBeDestroyed;
};

