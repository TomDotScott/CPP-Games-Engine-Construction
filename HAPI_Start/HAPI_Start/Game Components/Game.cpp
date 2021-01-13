#include "Game.h"
#include <fstream>
#include "../Audio/Music.h"
#include "../Audio/SoundManager.h"
#include "../State System/StateManager.h"

Game::Game() :
	State(),
	m_player({ Vector2::CENTRE }),
	m_levelTimer(200.f),
	m_totalElapsedTime(0.f),
	m_currentLevel(eLevel::e_LevelOne),
	m_gameClock(),
	m_levelStarted(false),
	m_levelFinished(false),
	m_leverPulled(false),
	m_flag(GenerateNextEntityID(), Vector2::ZERO),
	m_endLever(GenerateNextEntityID(), Vector2::ZERO),
	m_boss(GenerateNextEntityID(), {}, m_player),
	m_portal(GenerateNextEntityID(), {}),
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

	if (!m_levelFinished)
	{
		m_player.Update(deltaTime);

		switch (m_currentLevel)
		{
		case eLevel::e_LevelOne:
			if (m_player.GetPosition().x > constants::k_levelOneMaxX)
			{
				m_player.SetPosition({ constants::k_levelOneMaxX, m_player.GetPosition().y });
			}
			break;
		case eLevel::e_LevelTwo:
			if (m_player.GetPosition().x > constants::k_levelTwoMaxX)
			{
				m_player.SetPosition({ constants::k_levelTwoMaxX, m_player.GetPosition().y });
			}
			break;
		case eLevel::e_LevelThree:
			if (m_player.GetPosition().x > constants::k_levelThreeMaxX)
			{
				m_player.SetPosition({ constants::k_levelThreeMaxX, m_player.GetPosition().y });
			}
			break;
		default:
			break;
		}

		// If the player has touched the ground, start the level
		if (m_player.GetCurrentAlienState() == eAlienState::e_Idle && !m_levelStarted)
		{
			m_levelStarted = true;
		}

		// Kill the player if they are off screen
		if (m_player.GetPosition().y > constants::k_screenHeight)
		{
			// If level 1 and they fall off by the warp zone, take them there
			if (m_currentLevel == eLevel::e_LevelOne && (m_player.GetPosition().x > 4416.f && m_player.GetPosition().x <= 4608.f))
			{
				LoadLevel(eLevel::e_WarpZone, true, {Vector2::CENTRE.x, -constants::k_spriteSheetCellSize });
			} else
			{
				m_player.Kill();
			}
		}

		// Reset player if they died
		if (m_player.GetIsDead() && m_player.GetLivesRemaining() > 0)
		{
			LoadLevel(m_currentLevel, false);
		}

		if (m_player.GetCurrentAlienState() != eAlienState::e_Dead)
		{
			if (m_boss.GetActiveState())
			{
				m_boss.Update(deltaTime);
			}

			if (m_portal.GetActiveState())
			{
				m_portal.Update(deltaTime);
				if (m_portal.GetShouldTeleportPlayer())
				{
					LoadLevel(eLevel::e_LevelOne, true, { 6656.f, -constants::k_spriteSheetCellSize });
				}
			}

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

			ScrollBackground();
		}
	} else
	{
		if (m_player.GetCurrentAlienState() == eAlienState::e_Jumping)
		{
			// Slowly slide down
			m_player.SetPosition({ m_player.GetPosition().x, m_player.GetPosition().y + (200.f * deltaTime) });

			// Calculate the new flag position based on the player position
			const auto playerPos = m_player.GetPosition();

			const Vector2 newFlagPos{
				m_flag.GetPosition().x,
				playerPos.y
			};

			m_flag.SetPosition(newFlagPos);
		} else
		{
			const float newX = m_player.GetPosition().x + (500 * deltaTime);
			m_player.SetPosition({ newX, m_player.GetPosition().y });
		}

		m_player.PlayAnimation(deltaTime);
	}

	m_flag.Update(deltaTime);

	if (m_endLever.GetActiveState())
	{
		m_endLever.Update(deltaTime);

		if (!m_leverPulled)
		{
			if (m_endLever.GetLeverState() == eLeverState::e_Swinging)
			{
				m_leverPulled = true;
				m_tileManager.OnLeverPulled();
			}
		}
	}

	CheckCollisions();

	UpdateUI(deltaTime);

	m_gameClock = clock();

	// Update the music buffer so that music plays
	SoundManager::GetInstance().UpdateMusicBufferStream();

	if (m_player.GetLivesRemaining() == 0 || (m_levelTimer <= 0 && !m_levelFinished))
	{
		GameOver(false);
	}
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
	case eLevel::e_LevelThree:
		backgroundName = "Level3_Background";
		break;
	case eLevel::e_WarpZone:
		backgroundName = "WarpZone_Background";
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
		if (coin.GetActiveState())
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

	if (m_boss.GetActiveState())
	{
		m_boss.Render(textureManager, playerXOffset);
	}

	m_player.Render(textureManager);

	if (m_portal.GetActiveState())
	{
		m_portal.Render(textureManager, playerXOffset);
	}


	if (m_endLever.GetActiveState())
	{
		m_endLever.Render(textureManager, playerXOffset);
	}

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
		if (m_backgroundPosition.x < -constants::k_backgroundTileWidth)
		{
			m_backgroundPosition.x = 0;
		}
	} else if (m_player.GetCurrentDirection() == eDirection::e_Left && m_player.GetMoveDirectionLimit() != eDirection::e_Left)
	{
		m_backgroundPosition.x += 1.f;
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
		m_totalElapsedTime += deltaTime;
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
	}
	return string;
}

int Game::GenerateNextEntityID()
{
	static int currentEntityID = 0;
	return ++currentEntityID;
}

void Game::Input()
{
	if (m_levelStarted && !m_levelFinished)
	{
		// For ease of testing, press F1, F2 or F3 to go to
		// each level
		if (GetKey(eKeyCode::F1))
		{
			LoadLevel(eLevel::e_LevelOne);
		}

		if (GetKey(eKeyCode::F2))
		{
			LoadLevel(eLevel::e_LevelTwo);
		}

		if (GetKey(eKeyCode::F3))
		{
			LoadLevel(eLevel::e_LevelThree);
		}

		const HAPISPACE::HAPI_TControllerData& controllerData = HAPI.GetControllerData(0);
		
		if (GetKey(eKeyCode::W) || controllerData.digitalButtons[HK_DIGITAL_A])
		{
			if (m_player.GetCurrentAlienState() != eAlienState::e_Jumping)
			{
				m_player.SetShouldJump(true);
			}
		}

		if (GetKey(eKeyCode::SPACE) || controllerData.digitalButtons[HK_DIGITAL_B])
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
		if (GetKey(eKeyCode::A) || GetKey(eKeyCode::LEFT) || controllerData.digitalButtons[HK_DIGITAL_DPAD_LEFT])
		{
			playerMoveDir = eDirection::e_Left;
		} else if (GetKey(eKeyCode::D) || GetKey(eKeyCode::RIGHT) || controllerData.digitalButtons[HK_DIGITAL_DPAD_RIGHT])
		{
			playerMoveDir = eDirection::e_Right;
		}

		m_player.SetDirection(playerMoveDir);
	}
}

bool Game::Initialise(TextureManager& textureManager)
{
	// Load SFX
	SoundManager::GetInstance().AddSoundEffect("Block_Break", "Res/SFX/Block_Break.wav");
	SoundManager::GetInstance().AddSoundEffect("Block_Bump", "Res/SFX/Block_Bump.wav");
	SoundManager::GetInstance().AddSoundEffect("Brick_Break", "Res/SFX/Brick_Break.wav");
	SoundManager::GetInstance().AddSoundEffect("Coin", "Res/SFX/Coin.wav");
	SoundManager::GetInstance().AddSoundEffect("Entity_Shell_Hit", "Res/SFX/Entity_Shell_Hit.wav");
	SoundManager::GetInstance().AddSoundEffect("Entity_Squash", "Res/SFX/Entity_Squash.wav");
	SoundManager::GetInstance().AddSoundEffect("Entity_Fireball_Hit", "Res/SFX/Entity_Fireball_Hit.wav");
	SoundManager::GetInstance().AddSoundEffect("Fireball_Explosion", "Res/SFX/Fireball_Explosion.wav");
	SoundManager::GetInstance().AddSoundEffect("Fireball_Shoot", "Res/SFX/Fireball_Shoot.wav");
	SoundManager::GetInstance().AddSoundEffect("Fireball_Wall_Hit", "Res/SFX/Fireball_Wall_Hit.wav");
	SoundManager::GetInstance().AddSoundEffect("Flag", "Res/SFX/Flag.wav");
	SoundManager::GetInstance().AddSoundEffect("Player_Dead", "Res/SFX/Player_Dead.wav");
	SoundManager::GetInstance().AddSoundEffect("Player_Jump", "Res/SFX/Player_Jump.wav");
	SoundManager::GetInstance().AddSoundEffect("Player_Power_Down", "Res/SFX/Player_Power_Down.wav");
	SoundManager::GetInstance().AddSoundEffect("Player_Power_Up", "Res/SFX/Player_Power_Up.wav");
	SoundManager::GetInstance().AddSoundEffect("Power_Up_Reveal", "Res/SFX/Power_Up_Reveal.wav");
	SoundManager::GetInstance().AddSoundEffect("Shell_Hit_Wall", "Res/SFX/Shell_Hit_Wall.wav");

	SoundManager::GetInstance().AddMusic("Level1", "Res/Music/Level1.wav");
	SoundManager::GetInstance().AddMusic("Level2", "Res/Music/Level2.wav");
	SoundManager::GetInstance().AddMusic("Level3", "Res/Music/Level3.wav");
	SoundManager::GetInstance().AddMusic("WarpZone", "Res/Music/WarpZone.wav");

	//-----------------------TEXTURES-------------------------
	textureManager.CreateTexture("Res/Graphics/Level1_Background.tga", "Level1_Background");
	textureManager.CreateTexture("Res/Graphics/Level2_Background.tga", "Level2_Background");
	textureManager.CreateTexture("Res/Graphics/Level3_Background.tga", "Level3_Background");
	textureManager.CreateTexture("Res/Graphics/WarpZone_Background.tga", "WarpZone_Background");

	return LoadLevel(m_currentLevel);
}

bool Game::Unload(TextureManager& textureManager)
{
	// Remove all the assets
	if (!SoundManager::GetInstance().RemoveSoundEffect("Block_Break") ||
		!SoundManager::GetInstance().RemoveSoundEffect("Block_Bump") ||
		!SoundManager::GetInstance().RemoveSoundEffect("Brick_Break") ||
		!SoundManager::GetInstance().RemoveSoundEffect("Coin") ||
		!SoundManager::GetInstance().RemoveSoundEffect("Entity_Shell_Hit") ||
		!SoundManager::GetInstance().RemoveSoundEffect("Entity_Squash") ||
		!SoundManager::GetInstance().RemoveSoundEffect("Entity_Fireball_Hit") ||
		!SoundManager::GetInstance().RemoveSoundEffect("Fireball_Explosion") ||
		!SoundManager::GetInstance().RemoveSoundEffect("Fireball_Shoot") ||
		!SoundManager::GetInstance().RemoveSoundEffect("Fireball_Wall_Hit") ||
		!SoundManager::GetInstance().RemoveSoundEffect("Flag") ||
		!SoundManager::GetInstance().RemoveSoundEffect("Player_Dead") ||
		!SoundManager::GetInstance().RemoveSoundEffect("Player_Jump") ||
		!SoundManager::GetInstance().RemoveSoundEffect("Player_Power_Down") ||
		!SoundManager::GetInstance().RemoveSoundEffect("Player_Power_Up") ||
		!SoundManager::GetInstance().RemoveSoundEffect("Power_Up_Reveal") ||
		!SoundManager::GetInstance().RemoveSoundEffect("Shell_Hit_Wall") ||
		!SoundManager::GetInstance().RemoveMusic("Level1") ||
		!SoundManager::GetInstance().RemoveMusic("Level2") ||
		!SoundManager::GetInstance().RemoveMusic("Level3") ||
		!textureManager.RemoveTexture("Level1_Background") ||
		!textureManager.RemoveTexture("Level2_Background") ||
		!textureManager.RemoveTexture("Level3_Background"))
	{
		HAPI.UserMessage("Failed to unload the game assets", "An error occurred");
		return false;
	}
	return true;
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
		if (!LoadLevel(eLevel::e_LevelThree))
		{
			HAPI.UserMessage(("Level Three could not be loaded"), "An Error Occured");
			HAPI.Close();
		}
		break;
	case eLevel::e_LevelThree:
		GameOver(true);
		break;
	default:
		break;
	}
}

bool Game::LoadLevel(const eLevel level, const bool playerWon, const Vector2& playerPosition)
{
	// Clear the entity containers
	m_slimes.clear();
	m_snails.clear();
	m_coins.clear();
	m_gems.clear();

	m_boss.SetActiveState(false);
	m_endLever.SetActiveState(false);
	m_portal.SetActiveState(false);

	// Load the level
	std::string name;
	switch (level)
	{
	case eLevel::e_LevelOne:
		name = "Level1";
		m_levelTimer = 180.f;
		m_worldText.SetString("Level 1");
		break;
	case eLevel::e_LevelTwo:
		name = "Level2";
		m_levelTimer = 240.f;
		m_worldText.SetString("Level 2");
		break;
	case eLevel::e_LevelThree:
		name = "Level3";
		m_levelTimer = 300.f;
		m_worldText.SetString("Level 3");
		m_boss.SetActiveState(true);
		m_boss.SetBattleStarted(false);
		m_endLever.SetActiveState(true);
		break;
	case eLevel::e_WarpZone:
		name = "WarpZone";
		m_portal.SetActiveState(true);
		break;
	}

	if (!m_tileManager.LoadLevel("Res/Levels/" + name + ".csv"))
	{
		HAPI.UserMessage("Level data for " + name + " could not be loaded", "An Error Occurred");
		return false;
	}

	if (name != SoundManager::GetInstance().GetCurrentMusicIdentifier())
	{
		// Load music
		SoundManager::GetInstance().PlayMusic(name);
	}

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
		case eEntityType::e_Lever:
			m_endLever.SetPosition(entity.second);
			break;
		case eEntityType::e_Boss:
			m_boss.SetPosition(entity.second);
			break;
		case eEntityType::e_Portal:
			m_portal.SetPosition(entity.second);
			break;

		case eEntityType::e_None:
		case eEntityType::e_Player:
		case eEntityType::e_Fireball:
		case eEntityType::e_FireGem:
		case eEntityType::e_GrowGem:
			break;
		}
	}

	entityLocations.clear();

	// Reset Player
	m_player.Reset(playerWon, playerPosition);

	m_levelStarted = false;
	m_levelFinished = false;

	m_currentLevel = level;

	return true;
}

void Game::GameOver(const bool playerWon) const
{
	std::ifstream read("Res/score.txt");
	if (!read.is_open())
	{
		HAPI.UserMessage("Cannot open score text file", "An error occured");
	}

	std::string score;
	std::string highScore;
	std::string elapsedTime;

	std::getline(read, score);

	std::getline(read, highScore);

	std::getline(read, elapsedTime);

	score = std::to_string(m_player.GetScore());

	// if there is no highscore, the score IS the highscore
	if (highScore.empty())
	{
		highScore = score;
	} else
	{
		if (std::stoi(highScore) < m_player.GetScore())
		{
			highScore = score;
		}
	}

	elapsedTime = std::to_string(static_cast<int>(m_totalElapsedTime));

	read.close();

	std::ofstream write("Res/score.txt");

	write << AddLeadingZeroes(score, 6) << "\n" << AddLeadingZeroes(highScore, 6) << "\n" <<
		AddLeadingZeroes(elapsedTime, 3) << "\n";

	write << (playerWon ? "WIN" : "LOSE") << std::endl;

	write.close();

	STATE_MANAGER.ChangeState(eState::e_GameOver);
}

void Game::CheckCollisions()
{
	// CHECK ENTITY-LEVEL COLLISIONS
	if (m_player.GetCurrentAlienState() != eAlienState::e_Dead)
	{
		HandlePlayerCollisions();
	}
	if (!m_levelFinished)
	{
		if (m_tileManager.IsBossOnFloor(m_boss))
		{
			m_boss.SetAlienState(m_boss.GetBattleStarted() ? eAlienState::e_Walking : eAlienState::e_Idle);
		} else
		{
			m_boss.SetAlienState(eAlienState::e_Jumping);
		}

		m_player.CheckEntityCollisions(m_boss);

		// Check the collisions of the boss' active fireballs
		for (auto& fireball : m_boss.GetFireBallPool())
		{
			if (fireball.GetActiveState())
			{
				m_tileManager.CheckFireballLevelCollisions(fireball);
				m_player.CheckEntityCollisions(fireball);
			}
		}

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
				m_player.CheckEntityCollisions(pickup);
				pickup.CheckEntityCollisions(m_player);
			}
		}

		if (m_portal.GetActiveState())
		{
			m_portal.CheckEntityCollisions(m_player);
		}

		if (m_endLever.GetActiveState())
		{
			m_endLever.CheckEntityCollisions(m_player);
		}
	}
}


void Game::HandlePlayerCollisions()
{
	CollisionData& playerCollisionData = m_tileManager.CheckAlienLevelCollisions(m_player);

	// HEAD COLLISIONS
	if (playerCollisionData.m_topCollision)
	{
		// Stop the jump
		m_player.SetVelocity({ m_player.GetVelocity().x });

		// Work out which tile it was
		switch (playerCollisionData.m_topCollision->m_type)
		{
		case eTileType::e_CrateBlock:
			if (playerCollisionData.m_topCollision->m_canBeDestroyed)
			{
				if (m_player.GetPowerUpState() != ePowerUpState::e_Small)
				{
					playerCollisionData.m_topCollision->m_type = eTileType::e_Air;
					playerCollisionData.m_topCollision->m_canCollide = false;
					SoundManager::GetInstance().PlaySoundEffect("Block_Break");
				}
			}
			break;

		case eTileType::e_CoinBlock:
			playerCollisionData.m_topCollision->m_type = eTileType::e_CrateBlock;
			playerCollisionData.m_topCollision->m_canBeDestroyed = false;

			m_coins.emplace_back(
				GenerateNextEntityID(),
				Vector2(playerCollisionData.m_topCollision->m_position.x,
					playerCollisionData.m_topCollision->m_position.y -
					constants::k_spriteSheetCellSize),
				true
			);

			SoundManager::GetInstance().PlaySoundEffect("Coin");
			break;

		case eTileType::e_BoxedCoinBlock:
			playerCollisionData.m_topCollision->m_type = eTileType::e_CoinBlock;
			SoundManager::GetInstance().PlaySoundEffect("Brick_Break");
			break;

		case eTileType::e_ItemBlock:

			playerCollisionData.m_topCollision->m_type = eTileType::e_BrickBlock;
			SoundManager::GetInstance().PlaySoundEffect("Power_Up_Reveal");

			m_gems.emplace_back(
				GenerateNextEntityID(),
				Vector2(playerCollisionData.m_topCollision->m_position.x,
					playerCollisionData.m_topCollision->m_position.y -
					constants::k_spriteSheetCellSize),
				constants::rand_range(0, 100) <= 50
				? ePowerUpType::e_FireThrower
				: ePowerUpType::e_Grower,
				true
			);
			break;
		default:
			break;
		}

		// Resolve the overlap
		// Work out the overlap
		Tile* tile = playerCollisionData.m_topCollision;
		const Vector2 bottomOfTile = tile->m_tileCollisionBox.BOTTOM_RIGHT;

		const float distanceFromBottomOfTileToPlayerHead =
			bottomOfTile.y - m_player.GetCurrentCollisionBoxes().m_topCollisionBox.TOP_LEFT.y;

		// If the overlap is greater than 0 then we need to resolve the collision
		if (distanceFromBottomOfTileToPlayerHead > 0)
		{
			// Adding to move down the screen
			m_player.SetPosition({
									 m_player.GetPosition().x,
									 m_player.GetPosition().y + distanceFromBottomOfTileToPlayerHead
				});
		}
	}

	// Don't limit direction to start, as Left and Right collisions will
	// limit the player's move direction
	m_player.SetMoveDirectionLimit(eDirection::e_None);

	if (playerCollisionData.m_leftCollision)
	{
		m_player.SetMoveDirectionLimit(eDirection::e_Left);

		// Resolve the overlap to the left
		// Work out the overlap
		Tile* tile = playerCollisionData.m_leftCollision;
		const Vector2 topRightOfTile = { tile->m_tileCollisionBox.BOTTOM_RIGHT.x, tile->m_tileCollisionBox.TOP_LEFT.y };

		const float distanceFromTopRightOfTileToPlayerLeft =
			topRightOfTile.x - m_player.GetCurrentCollisionBoxes().m_leftCollisionBox.TOP_LEFT.x;

		// If the overlap is greater than 0 then we need to resolve the collision
		if (distanceFromTopRightOfTileToPlayerLeft > 0)
		{
			m_player.SetPosition({
									 m_player.GetPosition().x + distanceFromTopRightOfTileToPlayerLeft,
									 m_player.GetPosition().y
				});
		}
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


		if (!m_levelFinished)
		{
			// Resolve the overlap to the right
			// Work out the overlap
			Tile* tile = playerCollisionData.m_rightCollision;
			const Vector2 topLeftOfTile = tile->m_tileCollisionBox.TOP_LEFT;

			const float distanceFromTopLeftOfTileToPlayerRight =
				m_player.GetCurrentCollisionBoxes().m_rightCollisionBox.BOTTOM_RIGHT.x - topLeftOfTile.x;

			// If the overlap is greater than 0 then we need to resolve the collision
			if (distanceFromTopLeftOfTileToPlayerRight > 0)
			{
				m_player.SetPosition({
										 m_player.GetPosition().x - distanceFromTopLeftOfTileToPlayerRight,
										 m_player.GetPosition().y
					});
			}
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
				if (m_player.GetCurrentAlienState() != eAlienState::e_Dead)
				{
					m_player.SetAlienState(eAlienState::e_Walking);
				}
			}

			// Resolve the overlap on the bottom
			// Work out the overlap
			Tile* tile = playerCollisionData.m_bottomCollision;
			const Vector2 topLeftOfTile = tile->m_tileCollisionBox.TOP_LEFT;

			const float distanceFromTopLeftOfTileToPlayerBottom =
				m_player.GetCurrentCollisionBoxes().m_bottomCollisionBox.BOTTOM_RIGHT.y - topLeftOfTile.y;

			// If the overlap is greater than 0 then we need to resolve the collision
			if (distanceFromTopLeftOfTileToPlayerBottom > static_cast<float>(constants::k_spriteSheetCellSize) / 4.f)
			{
				m_player.SetPosition({
										 m_player.GetPosition().x,
										 m_player.GetPosition().y - distanceFromTopLeftOfTileToPlayerBottom
					});
			}

			m_player.SetAlienState(eAlienState::e_Walking);
		}
	} else
	{
		if (m_player.GetCurrentAlienState() != eAlienState::e_Dead)
		{
			// If there is no bottom collision, make the player fall
			m_player.SetAlienState(eAlienState::e_Jumping);
		}
	}
}
