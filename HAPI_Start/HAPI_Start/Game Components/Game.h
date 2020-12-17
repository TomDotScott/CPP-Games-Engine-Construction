#pragma once
#include <ctime>
#include <HAPI_lib.h>
#include <unordered_map>
#include "Entities/Coin.h"
#include "Entities/Enemies/Slime.h"
#include "Entities/Enemies/Snail.h"
#include "Entities/PickUpGem.h"
#include "Entities/Player.h"
#include "Text.h"
#include "TileManager.h"

enum class eKeyCode;
class Game
{
public:
	Game();
	void Update();
	void Render();
	static int GenerateNextEntityID();

	bool PLAYER_WON;
	bool PLAYER_LOST;

private:
	const HAPISPACE::HAPI_TKeyboardData& m_keyboardData;
	const HAPISPACE::HAPI_TControllerData& m_controllerData;

	TileManager m_tileManager;
	
	Player m_player;
	clock_t m_gameClock;
	
	int m_currentLevel;
	bool m_levelStarted;
	
	Vector2 m_backgroundPosition;
	eDirection m_backgroundMoveDir;

	std::vector<Slime> m_slimes;
	std::vector<Snail> m_snails;
	std::vector<Coin> m_coins;
	std::vector<PickUpGem> m_pickUpPool;

	Text m_scoreText;
	Text m_livesText;
	Text m_coinsText;
	Text m_worldText;
	Text m_timerText;

	static void CreateSprite(const std::string& spriteSheetIdentifier);
	float DeltaTime() const;
	bool GetKey(eKeyCode keyCode) const;
	void HandleKeyBoardInput();
	void HandleControllerInput();
	bool Initialise();
	bool LoadLevel(int levelNo);
	void ScrollBackground();
	void UpdateUI();
	static std::string AddLeadingZeroes(const std::string& string, int amountOfZeroes);
	template<typename T>
	void UpdateEnemies(std::vector<T>& enemies, float deltaTime);
	template<typename T>
	void CheckEnemyCollisions(std::vector<T>& enemies);
};

template <typename T>
void Game::UpdateEnemies(std::vector<T>& enemies, const float deltaTime)
{
	// Only work on snails and slimes...
	const float playerOffset = m_player.GetPosition().x;
	for (auto& enemy : enemies)
	{
		// Only update enemies if they're onscreen and alive
		if (enemy.GetPosition().x + static_cast<float>(constants::k_screenWidth) / 2.f - playerOffset < constants::k_screenWidth &&
			enemy.GetPosition().x + static_cast<float>(constants::k_screenWidth) / 2.f - playerOffset > 0)
		{
			enemy.Update(deltaTime);
		}
	}
}

template <typename T>
void Game::CheckEnemyCollisions(std::vector<T>& enemies)
{
	for (auto& enemy : enemies)
	{
		if (enemy.GetCurrentEntityState() != eEntityState::e_Dead)
		{
			m_player.CheckEntityCollisions(enemy);

			enemy.CheckEntityCollisions(m_player);

			// Check against the fireballs
			for (auto& fireball : m_player.GetFireBallPool())
			{
				if (fireball.GetActiveState())
				{
					enemy.CheckEntityCollisions(fireball);
				}
			}

			// Check for snail shell collisions
			for (auto& snail : m_snails)
			{
				if (snail.GetSnailState() == eSnailState::e_Sliding)
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

enum class eKeyCode
{
	NONE = -1, SHIFT = 16, SPACE = 32, LEFT = 37, UP = 38, RIGHT = 39, DOWN = 40, A = 65, B = 66, C = 67, D = 68, G = 71, P = 80, R = 82, S = 83, W = 87, Y = 89
};
