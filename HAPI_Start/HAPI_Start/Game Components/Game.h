#pragma once
#include <ctime>
#include <HAPI_lib.h>
#include <unordered_map>
#include "Enemies/Slime.h"
#include "Enemies/Snail.h"
#include "Player.h"

enum class e_KeyCode;
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
	e_Direction m_backgroundMoveDir;
	
	std::vector<std::vector<Tile>> m_levelData;

	std::vector<std::pair<e_EntityType, Vector2>> m_enemyLocations;
	std::vector<Slime> m_slimes;
	std::vector<Snail> m_snails;
	
	void CreateSprite(const std::string& spriteSheetIdentifier);
	float DeltaTime() const;
	bool GetKey(e_KeyCode keyCode) const;
	
	void HandleKeyBoardInput();
	void HandleControllerInput();

	bool Initialise();
	bool LoadLevel();
	void CheckPlayerLevelCollisions(CollisionBoxes playerCollisionBoxes);
	void CheckEnemyLevelCollisions(Enemy* enemy);
	template<typename T>
	void UpdateEntities(std::vector<T>& entityContainer, float deltaTime);
	void DrawTiles(int playerXOffset);
};

template <typename T>
void Game::UpdateEntities(std::vector<T>& entityContainer, const float deltaTime) {
	// Only work on snails and slimes...
	if (dynamic_cast<Snail*>(&entityContainer[0]) || dynamic_cast<Slime*>(&entityContainer[0])) {
		const float playerOffset = m_player.GetPosition().x;
		for (auto& enemy : entityContainer) {
			// Only update enemies if they're onscreen and alive
			if ((enemy.GetPosition().x + (static_cast<float>(constants::k_screenWidth) / 2.f) - playerOffset < constants::k_screenWidth)) {
				enemy.Update(deltaTime);
				if (enemy.GetCurrentEntityState() != e_EntityState::eDead) {
					CheckEnemyLevelCollisions(&enemy);
					m_player.CheckEntityCollisions(&enemy);
					enemy.CheckEntityCollisions(&m_player);
				}
			}
		}
	}
}

enum class e_KeyCode {
	NONE = -1, SPACE = 32, LEFT = 37, UP = 38, RIGHT = 39, DOWN = 40, A = 65, B = 66, C = 67, D = 68, G = 71, P = 80, R = 82, S = 83, W = 87, Y = 89
};

enum class e_TileType {
	eAir = -1, eDirt = 0, eGrassLeft = 1, eGrassCentre = 2,
	eGrassRight = 3, eStoneTop = 4, eStoneCentre = 5,
	eStoneLeft = 6, eStoneRight = 7, eFlag = 38,
	eCoinBlock = 47, eBoxedCoinBlock = 48, eCrateBlock = 49,
	eItemBlock = 50, eBrickBlock = 51, eBush = 52, eOpenDoorMid = 55,
	eOpenDoorTop = 56, ePlant = 57, eMushroom1 = 60, eMushroom2 = 61,
	eRock = 62, eSpikes = 63, eFlagPole = 64
};

struct Tile {
	Tile(const e_TileType type, const bool canCollide) :
		m_type(type),
		m_canCollide(canCollide) {
	}

	e_TileType m_type;
	bool m_canCollide;
};