#pragma once
#include <vector>
#include "../Utilities/Vector.h"
#include "Entities/Enemies/Enemy.h"
#include "Entities/Entity.h"
#include "Entities/Player.h"

class TextureManager;
struct Tile;

/**
 * \brief The Tiles that a given entity has collided with
 */
struct CollisionData
{
	void Reset()
	{
		m_topCollision   = nullptr;
		m_leftCollision   = nullptr;
		m_rightCollision  = nullptr;
		m_bottomCollision = nullptr;
	}

	Tile* m_topCollision;
	Tile* m_leftCollision;
	Tile* m_rightCollision;
	Tile* m_bottomCollision;
};

/**
 * \brief The TileManager is in charge of loading level data as well as
 * detecting collisions between the entities and the game world
 */
class TileManager
{
public:
	TileManager();
	/**
	 * \brief Loads level data from a .csv
	 * \param filename The file location of the .csv level data file relative to the working directory
	 * \return True if the level data was loaded successfully
	 */
	bool                                          LoadLevel(const std::string& filename);
	/**
	 * \brief Draws the tiles from the level data onscreen
	 * \param textureManager The texturemanager holds the sprite data
	 * needed to draw
	 * \param playerOffset The X (Horizontal) offset of the player
	 */
	void                                          RenderTiles(TextureManager& textureManager, float playerOffset);
	/**
	 * \brief Checks an alien's collisions with the world
	 * \param alien The desired alien
	 * \return The correct collision data
	 */
	CollisionData&                                CheckAlienLevelCollisions(Alien& alien);
	/**
	 * \brief Detects whether the player is on the bridge
	 * \param boss The boss entity
	 * \return True if the boss is on the floor
	 */
	bool                                          IsBossOnFloor(Alien& boss);
	/**
	 * \brief Trigger event that gets called when the player touches the trigger
	 */
	void                                          OnLeverPulled();
	/**
	 * \brief Checks the enemy collisions within the game world
	 * \param enemy The enemy to be checked
	 */
	void                                          CheckEnemyLevelCollisions(Enemy& enemy);
	/**
	 * \brief Checks the fireball collisions within the game world
	 * and determines whether it should explode if it hits on the
	 * left or right
	 * \param fireball The fireball entity to be checked 
	 */
	void                                          CheckFireballLevelCollisions(Fireball& fireball);
	/**
	 * \brief Gets the std::vector of entity locations from the level
	 * data
	 * \return The vector of entity locations
	 */
	std::vector<std::pair<eEntityType, Vector2>>& GetEntityLocations();

private:
	std::vector<std::vector<Tile>>               m_levelData;
	std::vector<std::pair<eEntityType, Vector2>> m_entityLocations;
	CollisionData                                m_alienCollisionData;

	Tile* CheckAlienHeadCollisions  (Alien& alien, const CollisionBoxes& alienCollisionBoxes);
	Tile* CheckAlienLeftCollisions  (Alien& alien, const CollisionBoxes& alienCollisionBoxes);
	Tile* CheckAlienRightCollisions (Alien& alien, const CollisionBoxes& alienCollisionBoxes);
	Tile* CheckAlienBottomCollisions(Alien& alien, const CollisionBoxes& alienCollisionBoxes);
};

/**
 * \brief The types of tile present in the level data
 * loaded from the .csv. The values of each entry are
 * how they are represented in the files
 */
enum class eTileType
{
	e_Air = -1,
	e_Dirt = 0,
	e_GrassLeft = 1,
	e_GrassCentre = 2,
	e_GrassRight = 3,
	e_StoneTop = 4,
	e_Stone = 5,
	e_StoneLeft = 6,
	e_StoneRight = 7,
	e_Flag = 38,
	e_CoinBlock = 47,
	e_BoxedCoinBlock = 48,
	e_CrateBlock = 49,
	e_ItemBlock = 50,
	e_BrickBlock = 51,
	e_Bush = 52,
	e_ClosedDoorMid = 53,
	e_ClosedDoorTop = 54,
	e_OpenDoorMid = 55,
	e_OpenDoorTop = 56,
	e_Plant = 57,
	e_Mushroom1 = 60,
	e_Mushroom2 = 61,
	e_Rock = 62,
	e_Spikes = 63,
	e_FlagPole = 64,
	e_Slime = 65,
	e_Coin = 66,
	e_Snail = 68,
	e_RightArrow = 69,
	e_GrassCliffLeft = 70,
	e_GrassCliffRight = 71,
	e_Boss = 73,
	e_Bridge = 74,
	e_Lever = 75,
	e_StoneCentre = 76,
	e_DestructibleStone = 77,
	e_Portal = 78,
	e_SecretSign = 79
};

inline const char* to_string(const eTileType e)
{
	switch (e)
	{
		case eTileType::e_Air: return "e_Air";
		case eTileType::e_Dirt: return "e_Dirt";
		case eTileType::e_GrassLeft: return "e_GrassLeft";
		case eTileType::e_GrassCentre: return "e_GrassCentre";
		case eTileType::e_GrassRight: return "e_GrassRight";
		case eTileType::e_StoneTop: return "e_StoneTop";
		case eTileType::e_Stone: return "e_Stone";
		case eTileType::e_StoneLeft: return "e_StoneLeft";
		case eTileType::e_StoneRight: return "e_StoneRight";
		case eTileType::e_Flag: return "e_Flag";
		case eTileType::e_CoinBlock: return "e_CoinBlock";
		case eTileType::e_BoxedCoinBlock: return "e_BoxedCoinBlock";
		case eTileType::e_CrateBlock: return "e_CrateBlock";
		case eTileType::e_ItemBlock: return "e_ItemBlock";
		case eTileType::e_BrickBlock: return "e_BrickBlock";
		case eTileType::e_Bush: return "e_Bush";
		case eTileType::e_ClosedDoorMid: return "e_ClosedDoorMid";
		case eTileType::e_ClosedDoorTop: return "e_ClosedDoorTop";
		case eTileType::e_OpenDoorMid: return "e_OpenDoorMid";
		case eTileType::e_OpenDoorTop: return "e_OpenDoorTop";
		case eTileType::e_Plant: return "e_Plant";
		case eTileType::e_Mushroom1: return "e_Mushroom1";
		case eTileType::e_Mushroom2: return "e_Mushroom2";
		case eTileType::e_Rock: return "e_Rock";
		case eTileType::e_Spikes: return "e_Spikes";
		case eTileType::e_FlagPole: return "e_FlagPole";
		case eTileType::e_Slime: return "e_Slime";
		case eTileType::e_Coin: return "e_Coin";
		case eTileType::e_Snail: return "e_Snail";
		case eTileType::e_RightArrow: return "e_RightArrow";
		case eTileType::e_GrassCliffLeft: return "e_GrassCliffLeft";
		case eTileType::e_GrassCliffRight: return "e_GrassCliffRight";
		case eTileType::e_Boss: return "e_Boss";
		case eTileType::e_Bridge: return "e_Bridge";
		case eTileType::e_Lever: return "e_Lever";
		case eTileType::e_StoneCentre: return "e_StoneCentre";
		case eTileType::e_DestructibleStone: return "e_DestructibleStone";
		case eTileType::e_Portal: return "e_Portal";
		case eTileType::e_SecretSign: return "e_SecretSign";
	}
	return "Unknown";
}

/**
 * \brief The Tile struct is the outline of each of the tiles
 * within the game world
 */
struct Tile
{
	/**
	 * \brief Constructs a tile object
	 * \param type The type of tile to be initialised
	 * \param position The position of the tile in the game world
	 * \param canCollide Whether entities are able to collide with
	 * the tile
	 */
	Tile(const eTileType type, const Vector2 position, const bool canCollide) :
		m_type(type),
		m_position(position),
		m_canCollide(canCollide),
		m_canBeDestroyed(true)
	{
		if (m_type != eTileType::e_Spikes)
		{
			m_tileCollisionBox = {
				m_position,
				{m_position.x + constants::k_spriteSheetCellSize, m_position.y + constants::k_spriteSheetCellSize}
			};
		}
		else
		{
			// Make spikes half sized
			m_tileCollisionBox = {
				{m_position.x, m_position.y + static_cast<float>(constants::k_spriteSheetCellSize) / 2.f},
				{m_position.x + constants::k_spriteSheetCellSize, m_position.y + constants::k_spriteSheetCellSize}
			};
		}
	}

	friend bool operator==(Tile& lhs, Tile& rhs)
	{
		return lhs.m_type == rhs.m_type
		       && lhs.m_position == rhs.m_position
		       && lhs.m_canCollide == rhs.m_canCollide
		       && lhs.m_canBeDestroyed == rhs.m_canBeDestroyed;
	}

	friend bool operator!=(Tile& lhs, Tile& rhs)
	{
		return !(lhs == rhs);
	}

	eTileType    m_type;
	Vector2      m_position;
	CollisionBox m_tileCollisionBox;
	bool         m_canCollide;
	bool         m_canBeDestroyed;
};
