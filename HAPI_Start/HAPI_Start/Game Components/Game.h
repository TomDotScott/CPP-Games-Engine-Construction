#pragma once
#include <ctime>
#include <HAPI_lib.h>
#include "Player.h"
#include "Slime.h"

enum class EKeyCode;
struct Tile;

class Game {
public:
	Game();
	void Update();
	void Render();
	bool PLAYER_WON;
	bool PLAYER_LOST;

private:
	const HAPISPACE::HAPI_TKeyboardData& m_keyboardData;
	const HAPISPACE::HAPI_TControllerData& m_controllerData;
	Player m_player;
	clock_t m_gameClock;
	
	int m_gameScore;
	int m_currentSprite;

	Vector2 m_backgroundPosition;
	Direction m_backgroundMoveDir;
	
	std::vector<std::vector<Tile>> m_levelData;

	std::vector<Vector2> m_enemyLocations;
	std::vector<Slime> m_enemies;
	
	void CreateSprite(const std::string& spriteSheetIdentifier);
	float DeltaTime() const;
	bool GetKey(EKeyCode keyCode) const;
	
	void HandleKeyBoardInput();
	void HandleControllerInput();

	bool Initialise();
	bool LoadLevel();
	void CheckPlayerLevelCollisions(CollisionBoxes playerCollisionBoxes);
	void CheckEnemyLevelCollisions(Enemy* enemy);
	void DrawTiles(int playerXOffset);
};

enum class EKeyCode {
	NONE = -1, SPACE = 32, LEFT = 37, UP = 38, RIGHT = 39, DOWN = 40, A = 65, B = 66, C = 67, D = 68, G = 71, P = 80, R = 82, S = 83, W = 87, Y = 89
};

enum class ETileType {
	eAir = -1, eDirt = 0, eGrassLeft = 1, eGrassCentre = 2,
	eGrassRight = 3, eStoneTop = 4, eStoneCentre = 5,
	eStoneLeft = 6, eStoneRight = 7, eFlag = 38,
	eCoinBlock = 47, eBoxedCoinBlock = 48, eCrateBlock = 49,
	eItemBlock = 50, eBrickBlock = 51, eBush = 52, eOpenDoorMid = 55,
	eOpenDoorTop = 56, ePlant = 57, eMushroom1 = 60, eMushroom2 = 61,
	eRock = 62, eSpikes = 63, eFlagPole = 64
};

struct Tile {
	Tile(const ETileType type, const bool canCollide) :
		m_type(type),
		m_canCollide(canCollide) {
	}

	ETileType m_type;
	bool m_canCollide;
};