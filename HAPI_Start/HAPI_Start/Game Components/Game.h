#pragma once
#include <ctime>
#include <unordered_map>

#include "Text.h"
#include "TileManager.h"
#include "../Graphics/TextureManager.h"
#include "../State System/State.h"
#include "Entities/Boss.h"
#include "Entities/Coin.h"
#include "Entities/Flag.h"
#include "Entities/Lever.h"
#include "Entities/PickUpGem.h"
#include "Entities/Player.h"
#include "Entities/Portal.h"
#include "Entities/Enemies/Slime.h"
#include "Entities/Enemies/Snail.h"

/**
 * \brief The Game class is the state that contains the world
 * elements and the entities and controls the playing of the game
 * demo. 
 */
class Game final : public State
{
public:
	Game();

	bool Initialise(TextureManager& textureManager) override;
	bool Unload(TextureManager& textureManager) override;
	void Update() override;
	void Render(TextureManager& textureManager) override;

	/**
	 * \brief Each entity needs a unique entity ID
	 * \return The next available ID for any entities that need to
	 * be instantiated
	 */
	static int GenerateNextEntityID();

private:
	/**
	 * \brief Each level of the game, the enum's value
	 * determines the music, level data and background to
	 * be loaded in Game::LoadLevel()
	 */
	enum class eLevel
	{
		e_LevelOne,
		e_LevelTwo,
		e_LevelThree,
		e_WarpZone
	};

	TileManager m_tileManager;

	float   m_levelTimer;
	float   m_totalElapsedTime;
	eLevel  m_currentLevel;
	clock_t m_gameClock;

	bool m_levelStarted;
	bool m_levelFinished;
	bool m_leverPulled;

	Vector2    m_backgroundPosition;

	// ENTITIES:
	Player  m_player;
	std::vector<Slime>     m_slimes;
	std::vector<Snail>     m_snails;
	std::vector<Coin>      m_coins;
	std::vector<PickUpGem> m_gems;
	Flag                   m_flag;
	Lever                  m_endLever;
	Boss                   m_boss;
	Portal                 m_portal;

	// UI TEXT:
	Text m_scoreText;
	Text m_livesText;
	Text m_coinsText;
	Text m_worldText;
	Text m_timerText;

	void               Input() override;
	/**
	 * \brief Using the TileManager class, the appropriate level
	 * data is loaded from the .csv file
	 * \param level The level to be loaded
	 * \param playerWon False if the player's power up state
	 * should be reset
	 * \param playerPosition The position that the player should be
	 * placed in the level
	 * \return True if the level was loaded correctly
	 */
	bool               LoadLevel(eLevel level, 
								 bool playerWon = true, 
								 const Vector2& playerPosition = Vector2(Vector2::CENTRE.x, -constants::k_spriteSheetCellSize));
	/**
	 * \brief Loads the next level of the game
	 */
	void               LoadNextLevel();
	/**
	 * \brief Unloads all of the assets, saves the scores and triggers
	 * the GameOver state
	 * \param playerWon True if the player completed the final level
	 */
	void               GameOver(bool playerWon) const;
	/**
	 * \brief Checks collisions between entities and between entities and the
	 * level
	 */
	void               CheckCollisions();
	/**
	 * \brief Handles the player collisions and the behaviours that should occur
	 * with the various sides of collision
	 */
	void               HandlePlayerCollisions();
	/**
	 * \brief Scrolls the background to the left or the right
	 * depending on player position
	 */
	void               ScrollBackground();
	/**
	 * \brief Updates the various UI elements of the game
	 * \param deltaTime The time between Game::Update() calls,
	 * used to countdown the level timer 
	 */
	void               UpdateUI(float deltaTime);
	/**
	 * \brief Adds leading zeroes to the given string, used in
	 * displaying the UI elements. e.g. 10 would
	 * become 0000010
	 * \param string The string that the 0s should be appended
	 * to
	 * \param amountOfZeroes The length of the string after the
	 * zeroes have been added
	 * \return The amended string
	 */
	static std::string AddLeadingZeroes(const std::string& string, int amountOfZeroes);
	/**
	 * \brief Updates the Enemy containers
	 * \tparam T The type of enemy
	 * \param enemies The container of enemies
	 * \param deltaTime The time difference between Game::Update()
	 * calls
	 */
	template <typename T>
	void UpdateEnemies(std::vector<T>& enemies, float deltaTime);
	/**
	 * \brief Checks enemy collisions using their containers
	 * \tparam T The type of enemy
	 * \param enemies The container of enemies
	 */
	template <typename T>
	void CheckEnemyCollisions(std::vector<T>& enemies);
};

template <typename T>
void Game::UpdateEnemies(std::vector<T>& enemies, const float deltaTime)
{
	const float playerOffset = m_player.GetPosition().x;
	for (auto& enemy : enemies)
	{
		// Only update enemies if they're onscreen and alive
		if (enemy.GetPosition().x + static_cast<float>(constants::k_screenWidth) / 2.f - playerOffset <
		    constants::k_screenWidth * 1.5f &&
		    enemy.GetPosition().x + static_cast<float>(constants::k_screenWidth) / 2.f - playerOffset > -
		    constants::k_screenWidth * 1.5f)
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
