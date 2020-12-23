#include "Game.h"
#include <fstream>
#include "../Audio/Music.h"
#include "../Audio/SoundManager.h"

Game::Game(const HAPISPACE::HAPI_TKeyboardData& keyboardData, const HAPISPACE::HAPI_TControllerData& controllerData) :
	State(keyboardData, controllerData),
	m_player({ Vector2::CENTRE }),
	m_levelTimer(200.f),
	m_currentLevel(eLevel::e_LevelOne),
	m_gameClock(),
	m_levelStarted(false),
	m_levelFinished(false),
	m_backgroundPosition(Vector2::ZERO),
	m_backgroundMoveDir(eDirection::e_None),
	m_flag(GenerateNextEntityID(), Vector2::ZERO),
	m_scoreText("Score 000000", { 0, 10 }),
	m_livesText(std::to_string(m_player.GetLivesRemaining()), { 467, 10 }),
	m_coinsText("0", { 680, 10 }),
	m_worldText("Level-1", { 856, 10 }),
	m_timerText("100", { 1150, 10 })
{
}

void Game::Update()
{
	const float deltaTime = DeltaTime(m_gameClock);

	// TODO: MOVE THIS LOGIC TO THE STATE MANAGER
	/*if (m_levelStarted && !m_levelFinished)
	{
		Input();
		HandleControllerInput();
	}*/

	if (!m_levelFinished)
	{
		m_player.Update(deltaTime);

		// If the player has touched the ground, start the level
		if (m_player.GetCurrentPlayerState() == ePlayerState::e_Idle && !m_levelStarted)
		{
			m_levelStarted = true;
		}

		// Kill the player if they are off screen
		if (m_player.GetPosition().y > constants::k_maxTilesVertical * constants::k_spriteSheetCellSize)
		{
			m_player.Kill();
		}

		// Reset player position if they died
		if (m_player.GetIsDead() && m_player.GetLivesRemaining() > 0)
		{
			m_player.Reset();
		}
	} else
	{
		if (m_player.GetCurrentPlayerState() == ePlayerState::e_Jumping)
		{
			// Slowly slide down
			m_player.SetPosition({ m_player.GetPosition().x, m_player.GetPosition().y + (200 * deltaTime) });

			// Calculate the new flag position based on the player position
			const auto playerPos = m_player.GetPosition();

			const Vector2 newFlagPos{
				static_cast<float>(((static_cast<int>(m_player.GetPosition().x) / constants::k_spriteSheetCellSize) + 1) * constants::k_spriteSheetCellSize),
				playerPos.y
			};

			m_flag.SetPosition(newFlagPos);
		} else
		{
			m_player.SetPosition({ m_player.GetPosition().x + (500 * deltaTime), m_player.GetPosition().y });
		}

		m_player.PlayAnimation(deltaTime);
	}

	m_flag.Update(deltaTime);

	UpdateEnemies(m_slimes, deltaTime);
	UpdateEnemies(m_snails, deltaTime);
	UpdateEnemies(m_coins, deltaTime);

	// UPDATE ANY ACTIVE PICKUPS
	for (auto& pickup : m_gems)
	{
		if (pickup.GetActiveState())
		{
			pickup.Update(deltaTime);
		}
	}

	CheckCollisions();

	ScrollBackground();

	UpdateUI(deltaTime);

	m_gameClock = clock();

	// Update the music buffer so that music plays
	SoundManager::GetInstance().UpdateMusicBufferStream();
}

void Game::Render(TextureManager& textureManager)
{
	textureManager.ClearScreen();

	std::string backgroundName;
	switch (m_currentLevel)
	{
	case eLevel::e_LevelOne:
		backgroundName = "Level1_Background";
		break;
	case eLevel::e_LevelTwo:
		backgroundName = "Level2_Background";
		break;
	default:
		break;
	}

	textureManager.DrawTexture(backgroundName, { m_backgroundPosition.x - constants::k_backgroundTileWidth, 0 });
	textureManager.DrawTexture(backgroundName, { m_backgroundPosition.x - 2 * constants::k_backgroundTileWidth, 0 });
	textureManager.DrawTexture(backgroundName, m_backgroundPosition);
	textureManager.DrawTexture(backgroundName, { m_backgroundPosition.x + constants::k_backgroundTileWidth, 0 });
	textureManager.DrawTexture(backgroundName, { m_backgroundPosition.x + 2 * constants::k_backgroundTileWidth, 0 });

	const auto playerXOffset = m_player.GetPosition().x;

	m_flag.Render(textureManager, playerXOffset);

	m_tileManager.RenderTiles(textureManager, playerXOffset);

	for (auto& coin : m_coins)
	{
		if (coin.GetIsVisible())
		{
			coin.Render(textureManager, playerXOffset);
		}
	}

	for (auto& slime : m_slimes)
	{
		slime.Render(textureManager, playerXOffset);
	}

	for (auto& snail : m_snails)
	{
		snail.Render(textureManager, playerXOffset);
	}

	for (auto& ball : m_player.GetFireBallPool())
	{
		if (ball.GetActiveState())
		{
			ball.Render(textureManager, playerXOffset);
		}
	}

	for (auto& pickup : m_gems)
	{
		if (pickup.GetActiveState())
		{
			pickup.Render(textureManager, playerXOffset);
		}
	}

	m_player.Render(textureManager);

	// Render UI on top of everything else
	m_scoreText.Render(textureManager);

	textureManager.DrawSprite("UI_Lives", { 394, 10 });
	textureManager.DrawSprite("UI_X", { 435, 10 });
	m_livesText.Render(textureManager);

	textureManager.DrawSprite("UI_Coins", { 605, 10 });
	textureManager.DrawSprite("UI_X", { 647, 10 });
	m_coinsText.Render(textureManager);

	m_worldText.Render(textureManager);
	m_timerText.Render(textureManager);
}

void Game::ScrollBackground()
{
	// Scroll the background
	if (m_player.GetCurrentDirection() == eDirection::e_Right && m_player.GetMoveDirectionLimit() != eDirection::e_Right)
	{
		m_backgroundPosition.x -= 1.f;
		m_backgroundMoveDir = eDirection::e_Left;
		if (m_backgroundPosition.x < -constants::k_backgroundTileWidth)
		{
			m_backgroundPosition.x = 0;
		}
	} else if (m_player.GetCurrentDirection() == eDirection::e_Left && m_player.GetMoveDirectionLimit() != eDirection::e_Left)
	{
		m_backgroundPosition.x += 1.f;
		m_backgroundMoveDir = eDirection::e_Right;
		if (m_backgroundPosition.x > constants::k_backgroundTileWidth)
		{
			m_backgroundPosition.x = 0;
		}
	}
}

void Game::UpdateUI(const float deltaTime)
{
	// New Score Text...
	m_scoreText.SetString("Score " + AddLeadingZeroes(std::to_string(m_player.GetScore()), 6));

	// New Coins Text...
	m_coinsText.SetString(AddLeadingZeroes(std::to_string(m_player.GetCoinCount()), 2));

	// New lives Text...
	m_livesText.SetString(AddLeadingZeroes(std::to_string(m_player.GetLivesRemaining()), 2));

	if (m_levelStarted)
	{
		m_levelTimer -= deltaTime;
		const std::string timeRemaining = std::to_string(static_cast<int>(m_levelTimer));
		m_timerText.SetString(AddLeadingZeroes(timeRemaining, 3));
	}
}

std::string Game::AddLeadingZeroes(const std::string& string, const int amountOfZeroes)
{
	const int length = amountOfZeroes - static_cast<int>(string.length());
	if (length > 0)
	{
		return std::string(length, '0') + string;
	} else
	{
		return string;
	}
}

int Game::GenerateNextEntityID()
{
	static int currentEntityID = 0;
	return ++currentEntityID;
}

void Game::Input()
{
	if (GetKey(eKeyCode::SPACE))
	{
		if (m_player.GetCurrentPlayerState() != ePlayerState::e_Jumping)
		{
			m_player.SetShouldJump(true);
		}
	}

	if (GetKey(eKeyCode::W))
	{
		if (m_player.GetPowerUpState() == ePowerUpState::e_FireThrower)
		{
			m_player.SetCanShoot(true);
		}
	} else
	{
		m_player.SetCanShoot(false);
	}

	eDirection playerMoveDir = eDirection::e_None;
	if (GetKey(eKeyCode::A) || GetKey(eKeyCode::LEFT))
	{
		playerMoveDir = eDirection::e_Left;
	} else if (GetKey(eKeyCode::D) || GetKey(eKeyCode::RIGHT))
	{
		playerMoveDir = eDirection::e_Right;
	}

	m_player.SetDirection(playerMoveDir);
}

bool Game::Initialise(TextureManager& textureManager)
{
	SoundManager::GetInstance().Initialise();

	//-----------------------TEXTURES-------------------------
	textureManager.CreateTexture("Res/Graphics/Level1_Background.tga", "Level1_Background");
	textureManager.CreateTexture("Res/Graphics/Level2_Background.tga", "Level2_Background");

	return LoadLevel(eLevel::e_LevelOne);
}

void Game::LoadNextLevel()
{
	switch (m_currentLevel)
	{
	case eLevel::e_LevelOne:
		if (!LoadLevel(eLevel::e_LevelTwo))
		{
			HAPI.UserMessage(("Level Two could not be loaded"), "An Error Occured");
			HAPI.Close();
		}
		break;
	case eLevel::e_LevelTwo:
		break;
	default:
		break;
	}
}

bool Game::LoadLevel(const eLevel level)
{
	// Clear the entity containers
	m_slimes.clear();
	m_snails.clear();
	m_coins.clear();

	// Load the level
	std::string name;
	switch (level)
	{
	case eLevel::e_LevelOne:
		name = "Level1";
		m_worldText.SetString("Level 1");
		break;
	case eLevel::e_LevelTwo:
		name = "Level2";
		m_worldText.SetString("Level 2");
		break;
	default:
		HAPI.UserMessage(("Level not yet created"), "An Error Occured");
		return false;
	}

	if (!m_tileManager.LoadLevel("Res/Levels/" + name + ".csv"))
	{
		HAPI.UserMessage("Level data for " + name + " could not be loaded", "An Error Occurred");
		return false;
	}

	// Load music
	SoundManager::GetInstance().PlayMusic(name);

	// Spawn the enemies
	auto& entityLocations = m_tileManager.GetEntityLocations();

	// Dequeue every entity location
	for (auto& entity : entityLocations)
	{

		switch (entity.first)
		{
		case eEntityType::e_Coin:
			m_coins.emplace_back(GenerateNextEntityID(), entity.second, true);
			break;
		case eEntityType::e_Slime:
			m_slimes.emplace_back(GenerateNextEntityID(), entity.second, entity.second.y == 768.f ? true : false);
			break;
		case eEntityType::e_Snail:
			m_snails.emplace_back(GenerateNextEntityID(), entity.second);
			break;
		case eEntityType::e_Flag:
			m_flag.SetPosition(entity.second);
			break;
		default: break;
		}
	}

	entityLocations.clear();

	// Reset Player
	m_player.Reset();

	m_levelStarted = false;
	m_levelFinished = false;

	m_levelTimer = 200.f;

	return true;
}

void Game::CheckCollisions()
{
	// CHECK ENTITY-LEVEL COLLISIONS 
	HandlePlayerCollisions();

	for (auto& slime : m_slimes)
	{
		m_tileManager.CheckEnemyLevelCollisions(slime);
	}

	for (auto& snail : m_snails)
	{
		m_tileManager.CheckEnemyLevelCollisions(snail);
	}

	// CHECK ENTITY-ENTITY COLLISIONS
	CheckEnemyCollisions(m_slimes);
	CheckEnemyCollisions(m_snails);

	for (auto& c : m_coins)
	{
		c.CheckEntityCollisions(m_player);
	}

	for (auto& pickup : m_gems)
	{
		if (pickup.GetActiveState())
		{
			pickup.CheckEntityCollisions(m_player);
			m_player.CheckEntityCollisions(pickup);
		}
	}
}

void Game::HandlePlayerCollisions()
{
	CollisionData& playerCollisionData = m_tileManager.CheckPlayerLevelCollisions(m_player);

	// HEAD COLLISIONS
	if (playerCollisionData.m_headCollision)
	{
		// Stop the jump
		m_player.SetVelocity({ m_player.GetVelocity().x });

		// Work out which tile it was
		switch (playerCollisionData.m_headCollision->m_type)
		{
		case eTileType::e_CrateBlock:
			if (playerCollisionData.m_headCollision->m_canBeDestroyed)
			{
				playerCollisionData.m_headCollision->m_type = eTileType::e_Air;
				playerCollisionData.m_headCollision->m_canCollide = false;
				SoundManager::GetInstance().PlaySoundEffect("Block_Break");
			}
			break;

		case eTileType::e_CoinBlock:
			playerCollisionData.m_headCollision->m_type = eTileType::e_CrateBlock;
			playerCollisionData.m_headCollision->m_canBeDestroyed = false;

			m_coins.emplace_back(
				GenerateNextEntityID(),
				Vector2(playerCollisionData.m_headCollision->m_position.x, playerCollisionData.m_headCollision->m_position.y - constants::k_spriteSheetCellSize),
				true
			);

			SoundManager::GetInstance().PlaySoundEffect("Coin");
			break;

		case eTileType::e_BoxedCoinBlock:
			playerCollisionData.m_headCollision->m_type = eTileType::e_CoinBlock;
			SoundManager::GetInstance().PlaySoundEffect("Brick_Break");
			break;

		case eTileType::e_ItemBlock:
			playerCollisionData.m_headCollision->m_type = eTileType::e_BrickBlock;
			SoundManager::GetInstance().PlaySoundEffect("Power_Up_Reveal");
			m_gems.emplace_back(
				GenerateNextEntityID(),
				Vector2(playerCollisionData.m_headCollision->m_position.x, playerCollisionData.m_headCollision->m_position.y - constants::k_spriteSheetCellSize),
				constants::rand_range(0, 100) <= 50 ? ePowerUpType::e_FireThrower : ePowerUpType::e_Grower,
				true
			);
			break;
		default:
			break;
		}
	}

	if (playerCollisionData.m_leftCollision)
	{
		m_player.SetMoveDirectionLimit(eDirection::e_Left);
	}

	if (playerCollisionData.m_rightCollision)
	{
		switch (playerCollisionData.m_rightCollision->m_type)
		{
		case eTileType::e_OpenDoorMid:
		case eTileType::e_OpenDoorTop:
			LoadNextLevel();
			break;
		case eTileType::e_Flag:
			if (!m_levelFinished)
			{
				m_player.AddToScore(500); // 500 point bonus for hitting the flag at the top
			}
		case eTileType::e_FlagPole:
			if (!m_levelFinished)
			{
				m_player.AddToScore(500);
			}
			m_player.AddToScore(static_cast<int>(m_levelTimer));
			m_levelTimer = 0.f;
			m_levelFinished = true;
			break;
		default:
			m_player.SetMoveDirectionLimit(eDirection::e_Right);
		}
	}

	if (playerCollisionData.m_bottomCollision)
	{
		if (playerCollisionData.m_bottomCollision->m_type == eTileType::e_Spikes)
		{
			m_player.Kill();
		} else
		{
			if (playerCollisionData.m_bottomCollision->m_position.y > m_player.GetPosition().y)
			{
				if (m_player.GetCurrentPlayerState() != ePlayerState::e_Dead)
				{
					m_player.SetPlayerState(ePlayerState::e_Walking);
				}
			}
		}
	} else
	{
		if (m_player.GetCurrentPlayerState() != ePlayerState::e_Dead)
		{
			// If there is no bottom collision, make the player jump
			m_player.SetPlayerState(ePlayerState::e_Jumping);
		}
	}
}
