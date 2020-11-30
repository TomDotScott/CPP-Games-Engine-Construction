#pragma once
#include <ctime>
#include <HAPI_lib.h>
#include <unordered_map>
#include "Coin.h"
#include "Enemies/Slime.h"
#include "Enemies/Snail.h"
#include "Player.h"
#include "TileManager.h"

enum class e_KeyCode;

class Game
{
public:
	Game();
	void Update();
	void Render();
	bool PLAYER_WON;
	bool PLAYER_LOST;

private:
	const HAPISPACE::HAPI_TKeyboardData& m_keyboardData;
	const HAPISPACE::HAPI_TControllerData& m_controllerData;

	TileManager m_tileManager;
	
	Player m_player;
	clock_t m_gameClock;

	int m_gameScore;
	int m_currentSprite;
	
	Vector2 m_backgroundPosition;
	e_Direction m_backgroundMoveDir;

	std::vector<Slime> m_slimes;
	std::vector<Snail> m_snails;
	std::vector<Coin> m_coins;

	void CreateSprite(const std::string& spriteSheetIdentifier);
	
	float DeltaTime() const;

	bool GetKey(e_KeyCode keyCode) const;

	void HandleKeyBoardInput();
	void HandleControllerInput();

	bool Initialise();

	template<typename T>
	void UpdateEnemies(std::vector<T>& enemyContainer, float deltaTime);
};

template <typename T>
void Game::UpdateEnemies(std::vector<T>& enemyContainer, const float deltaTime)
{
	// Only work on snails and slimes...
	const float playerOffset = m_player.GetPosition().x;
	for (auto& enemy : enemyContainer)
	{
		// Only update enemies if they're onscreen and alive
		if (enemy.GetPosition().x + static_cast<float>(constants::k_screenWidth) / 2.f - playerOffset < constants::k_screenWidth &&
			enemy.GetPosition().x + static_cast<float>(constants::k_screenWidth) / 2.f - playerOffset > 0)
		{
			enemy.Update(deltaTime);
			m_tileManager.CheckEnemyLevelCollisions(enemy);
			if (enemy.GetCurrentEntityState() != e_EntityState::eDead)
			{
				m_player.CheckEntityCollisions(enemy);
				enemy.CheckEntityCollisions(m_player);
				// Check for snail shell collisions
				for (auto& snail : m_snails)
				{
					if (snail.GetSnailState() == e_SnailState::eSliding)
					{
						if (enemy.GetEntityID() != snail.GetEntityID())
						{
							enemy.CheckSnailShellCollisions(snail.GetCurrentCollisionBoxes());
						}
					}
				}
			}
		}
	}
}

enum class e_KeyCode
{
	NONE = -1, SPACE = 32, LEFT = 37, UP = 38, RIGHT = 39, DOWN = 40, A = 65, B = 66, C = 67, D = 68, G = 71, P = 80, R = 82, S = 83, W = 87, Y = 89
};