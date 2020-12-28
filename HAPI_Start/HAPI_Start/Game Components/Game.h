#pragma once
#include <ctime>
#include <HAPI_lib.h>
#include <unordered_map>

#include "../Graphics/TextureManager.h"
#include "../State System/State.h"
#include "Entities/Boss.h"
#include "Entities/Coin.h"
#include "Entities/Enemies/Slime.h"
#include "Entities/Enemies/Snail.h"
#include "Entities/Flag.h"
#include "Entities/PickUpGem.h"
#include "Entities/Player.h"
#include "Text.h"
#include "TileManager.h"

class Game final : public State
{
public:
	Game(const HAPISPACE::HAPI_TKeyboardData& keyboardData, const HAPISPACE::HAPI_TControllerData& controllerData);
	void Update() override;
	void Render(TextureManager& textureManager) override;
	static int GenerateNextEntityID();

private:
	enum class eLevel
	{
		e_LevelOne, e_LevelTwo, e_LevelThree
	};

	TileManager m_tileManager;
	
	Player m_player;
	float m_levelTimer;
	eLevel m_currentLevel;
	clock_t m_gameClock;
	
	bool m_levelStarted;
	bool m_levelFinished;
	
	Vector2 m_backgroundPosition;
	eDirection m_backgroundMoveDir;

	std::vector<Slime> m_slimes;
	std::vector<Snail> m_snails;
	std::vector<Coin> m_coins;
	std::vector<PickUpGem> m_gems;
	Flag m_flag;
	Boss m_boss;

	Text m_scoreText;
	Text m_livesText;
	Text m_coinsText;
	Text m_worldText;
	Text m_timerText;
	
	void Input() override;
	bool Initialise(TextureManager& textureManager) override;
	void LoadNextLevel();
	bool LoadLevel(eLevel level);
	void CheckCollisions();
	void HandlePlayerCollisions();
	void HandleBossCollisions();
	void ScrollBackground();
	void UpdateUI(float deltaTime);
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