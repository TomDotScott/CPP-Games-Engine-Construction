#include "Game.h"
#include <fstream>
#include "../Audio/Music.h"
#include "../Audio/SoundManager.h"
#include "../Graphics/Graphics.h"

Game::Game() :
	PLAYER_WON(false),
	PLAYER_LOST(false),
	m_keyboardData(HAPI.GetKeyboardData()),
	m_controllerData(HAPI.GetControllerData(0)),
	m_tileManager(),
	m_player({ Vector2::CENTRE }),
	m_gameClock(),
	m_levelTimer(200.f),
	m_currentLevel(eLevel::e_LevelOne),
	m_levelStarted(false),
	m_backgroundPosition(Vector2::ZERO),
	m_backgroundMoveDir(eDirection::e_None),
	m_scoreText("Score 000000", HAPISPACE::HAPI_TColour::WHITE, { 0, 10 }),
	m_livesText(std::to_string(m_player.GetLivesRemaining()), HAPISPACE::HAPI_TColour::WHITE, { 467, 10 }),
	m_coinsText("0", HAPISPACE::HAPI_TColour::WHITE, { 680, 10 }),
	m_worldText("Level-1", HAPISPACE::HAPI_TColour::WHITE, { 856, 10 }),
	m_timerText("100", HAPISPACE::HAPI_TColour::WHITE, { 1150, 10 })
{
	if (!Initialise())
	{
		HAPI.UserMessage("An error occured during initialisation", "AN ERROR OCCURED");
		HAPI.Close();
	}
}

void Game::Update()
{
	// See if the player has triggered the next level
	if (m_tileManager.ShouldLoadNextLevel())
	{
		LoadNextLevel();
	}

	const float deltaTime = DeltaTime();

	if (m_levelStarted)
	{
		HandleKeyBoardInput();
		HandleControllerInput();
	}

	// Reset player position if they died
	if (m_player.GetIsDead() && m_player.GetLivesRemaining() > 0)
	{
		m_player.Reset();
	}

	// SEE IF THERE ARE ANY COINS OR GEMS TO SET VISIBLE
	auto& newEntityLocations = m_tileManager.GetEntityLocations();
	while (!newEntityLocations.empty())
	{
		const auto& front = newEntityLocations.front();
		switch (front.first)
		{
		case eEntityType::e_FireGem:
		case eEntityType::e_GrowGem:

			for (auto& gem : m_pickUpPool)
			{
				if (gem.GetActiveState() == false)
				{
					if (front.first == eEntityType::e_FireGem)
					{
						gem.SetPowerUpType(ePowerUpType::e_FireThrower);
						std::cout << "Gem set to fire" << std::endl;
					} else
					{
						gem.SetPowerUpType(ePowerUpType::e_Grower);
						std::cout << "Gem set to grow" << std::endl;
					}
					gem.Initialise(front.second);
				}
			}
			break;

		default: break;
		}
		// dequeue the element
		newEntityLocations.pop();
	}


	// UPDATE ENTITIES
	m_player.Update(deltaTime);

	// If the player has touched the ground, start the level
	if (m_player.GetCurrentPlayerState() == ePlayerState::e_Idle && !m_levelStarted)
	{
		m_levelStarted = true;
	}


	UpdateEnemies(m_slimes, deltaTime);
	UpdateEnemies(m_snails, deltaTime);
	UpdateEnemies(m_coins, deltaTime);

	// UPDATE ANY ACTIVE PICKUPS
	for (auto& pickup : m_pickUpPool)
	{
		if (pickup.GetActiveState())
		{
			pickup.Update(deltaTime);
		}
	}


	// CHECK ENTITY-LEVEL COLLISIONS 
	m_tileManager.CheckPlayerLevelCollisions(m_player);
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

	for (auto& pickup : m_pickUpPool)
	{
		if (pickup.GetActiveState())
		{
			pickup.CheckEntityCollisions(m_player);
			m_player.CheckEntityCollisions(pickup);
		}
	}

	ScrollBackground();

	UpdateUI(deltaTime);

	// Reset the clock
	m_gameClock = clock();

	// Update the music buffer so that music plays
	SoundManager::GetInstance().UpdateMusicBufferStream();
}

void Game::Render()
{
	Graphics::GetInstance().ClearScreen();

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

	Graphics::GetInstance().DrawTexture(backgroundName, { m_backgroundPosition.x - constants::k_backgroundTileWidth, 0 });
	Graphics::GetInstance().DrawTexture(backgroundName, { m_backgroundPosition.x - 2 * constants::k_backgroundTileWidth, 0 });
	Graphics::GetInstance().DrawTexture(backgroundName, m_backgroundPosition);
	Graphics::GetInstance().DrawTexture(backgroundName, { m_backgroundPosition.x + constants::k_backgroundTileWidth, 0 });
	Graphics::GetInstance().DrawTexture(backgroundName, { m_backgroundPosition.x + 2 * constants::k_backgroundTileWidth, 0 });

	const auto playerXOffset = m_player.GetPosition().x;

	m_tileManager.RenderTiles(playerXOffset);

	for (auto& coin : m_coins)
	{
		if (coin.GetIsVisible())
		{
			coin.Render(playerXOffset);
		}
	}

	for (auto& slime : m_slimes)
	{
		slime.Render(playerXOffset);
	}

	for (auto& snail : m_snails)
	{
		snail.Render(playerXOffset);
	}

	for (auto& ball : m_player.GetFireBallPool())
	{
		if (ball.GetActiveState())
		{
			ball.Render(playerXOffset);
		}
	}

	for (auto& pickup : m_pickUpPool)
	{
		if (pickup.GetActiveState())
		{
			pickup.Render(playerXOffset);
		}
	}

	m_player.Render();

	// Render UI on top of everything else
	m_scoreText.Render();

	Graphics::GetInstance().DrawSprite("UI_Lives", { 394, 10 });
	Graphics::GetInstance().DrawSprite("UI_X", { 435, 10 });
	m_livesText.Render();

	Graphics::GetInstance().DrawSprite("UI_Coins", { 605, 10 });
	Graphics::GetInstance().DrawSprite("UI_X", { 647, 10 });
	m_coinsText.Render();

	m_worldText.Render();
	m_timerText.Render();
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

void Game::CreateSprite(const std::string& spriteSheetIdentifier)
{
	static int currentSpriteID = 0;
	if (!Graphics::GetInstance().CreateSprite(spriteSheetIdentifier, currentSpriteID))
	{
		HAPI.Close();
	}
	currentSpriteID++;
}

float Game::DeltaTime() const
{
	const clock_t programTickCount = clock() - m_gameClock;

	const float ticksToMilliseconds = 1000.f / CLOCKS_PER_SEC;

	return (programTickCount * ticksToMilliseconds) / 1000.f;
}

bool Game::GetKey(const eKeyCode keyCode) const
{
	return m_keyboardData.scanCode[static_cast<int>(keyCode)] ? true : false;
}

void Game::HandleKeyBoardInput()
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

void Game::HandleControllerInput()
{
	//Player One Controls 
	Vector2 leftStickVector{ static_cast<float>(m_controllerData.analogueButtons[2]), static_cast<float>(m_controllerData.analogueButtons[3]) };
	if (leftStickVector.Magnitude() > constants::k_leftThumbDeadzone)
	{
		leftStickVector.Normalised();
		if (leftStickVector.x > 0)
		{
			m_player.SetDirection(eDirection::e_Right);
		} else if (leftStickVector.x < 0)
		{
			m_player.SetDirection(eDirection::e_Left);
		} else
		{
			m_player.SetDirection(eDirection::e_None);
		}
	}
}

bool Game::Initialise()
{
	if (!Graphics::GetInstance().CreateTexture("Res/Graphics/Level1_Background.tga", "Level1_Background"))
	{
		HAPI.UserMessage("Level1 background couldn't be loaded", "An Error Occurred");
		return false;
	}

	if (!Graphics::GetInstance().CreateTexture("Res/Graphics/Level2_Background.tga", "Level2_Background"))
	{
		HAPI.UserMessage("Level1 background couldn't be loaded", "An Error Occurred");
		return false;
	}

	if (!Graphics::GetInstance().CreateSpriteSheet("Res/Graphics/GameSpriteSheetPixelised.tga"))
	{
		HAPI.UserMessage("Spritesheet Could Not Be Loaded", "An Error Occurred");
		return false;
	}

	if (!HAPI.ChangeFontFromFile("Res/Graphics/Font.ttf"))
	{
		HAPI.UserMessage("Font couldn't be loaded", "An Error Occurred");
		return false;
	}

	// Load the sprites 
	CreateSprite("Player_Small_Dead_Body");
	CreateSprite("Player_Small_Walk_Body_1");
	CreateSprite("Player_Small_Walk_Body_2");
	CreateSprite("Player_Small_Jump_Body_1");
	CreateSprite("Player_Small_Idle_Body_1");
	CreateSprite("Player_Small_Idle_Body_2");
	CreateSprite("Player_Dead_Top");
	CreateSprite("Player_Dead_Body");
	CreateSprite("Player_Walk_Top_1");
	CreateSprite("Player_Walk_Body_1");
	CreateSprite("Player_Walk_Top_2");
	CreateSprite("Player_Walk_Body_2");
	CreateSprite("Player_Jump_Top_1");
	CreateSprite("Player_Jump_Body_1");
	CreateSprite("Player_Idle_Top_1");
	CreateSprite("Player_Idle_Body_1");
	CreateSprite("Player_Idle_Top_2");
	CreateSprite("Player_Idle_Body_2");
	CreateSprite("Player_Climb_Top_1");
	CreateSprite("Player_Climb_Body_1");
	CreateSprite("Player_Climb_Top_2");
	CreateSprite("Player_Climb_Body_2");
	CreateSprite("Player_Power_Up_Dead_Top");
	CreateSprite("Player_Power_Up_Dead_Body");
	CreateSprite("Player_Power_Up_Walk_Top_1");
	CreateSprite("Player_Power_Up_Walk_Body_1");
	CreateSprite("Player_Power_Up_Walk_Top_2");
	CreateSprite("Player_Power_Up_Walk_Body_2");
	CreateSprite("Player_Power_Up_Jump_Top_1");
	CreateSprite("Player_Power_Up_Jump_Body_1");
	CreateSprite("Player_Power_Up_Idle_Top_1");
	CreateSprite("Player_Power_Up_Idle_Body_1");
	CreateSprite("Player_Power_Up_Idle_Top_2");
	CreateSprite("Player_Power_Up_Idle_Body_2");
	CreateSprite("Player_Power_Up_Climb_Top_1");
	CreateSprite("Player_Power_Up_Climb_Body_1");
	CreateSprite("Player_Power_Up_Climb_Top_2");
	CreateSprite("Player_Power_Up_Climb_Body_2");
	CreateSprite("Fireball_1");
	CreateSprite("Fireball_2");
	CreateSprite("Fireball_3");
	CreateSprite("Fireball_4");
	CreateSprite("Explosion_1");
	CreateSprite("Explosion_2");
	CreateSprite("Explosion_3");
	CreateSprite("Explosion_4");
	CreateSprite("Explosion_5");
	CreateSprite("Explosion_6");
	CreateSprite("Explosion_7");
	CreateSprite("Explosion_8");
	CreateSprite("Grass_Left");
	CreateSprite("Grass_Centre");
	CreateSprite("Grass_Right");
	CreateSprite("Dirt");
	CreateSprite("Grass_Cliff_Left");
	CreateSprite("Grass_Cliff_Right");
	CreateSprite("Stone_Left");
	CreateSprite("Stone_Centre");
	CreateSprite("Stone_Right");
	CreateSprite("Stone_Top");
	CreateSprite("Coin_1");
	CreateSprite("Coin_2");
	CreateSprite("Coin_3");
	CreateSprite("Coin_4");
	CreateSprite("Coin_5");
	CreateSprite("Coin_6");
	CreateSprite("Coin_7");
	CreateSprite("Coin_8");
	CreateSprite("Gem_Fire_1");
	CreateSprite("Gem_Fire_2");
	CreateSprite("Gem_Fire_3");
	CreateSprite("Gem_Fire_4");
	CreateSprite("Gem_Fire_5");
	CreateSprite("Gem_Fire_6");
	CreateSprite("Gem_Grow_1");
	CreateSprite("Gem_Grow_2");
	CreateSprite("Gem_Grow_3");
	CreateSprite("Gem_Grow_4");
	CreateSprite("Gem_Grow_5");
	CreateSprite("Gem_Grow_6");
	CreateSprite("Flag_Up_1");
	CreateSprite("Flag_Up_2");
	CreateSprite("Flag_Down");
	CreateSprite("Flag_Pole");
	CreateSprite("UI_0");
	CreateSprite("UI_1");
	CreateSprite("UI_2");
	CreateSprite("UI_3");
	CreateSprite("UI_4");
	CreateSprite("UI_5");
	CreateSprite("UI_6");
	CreateSprite("UI_7");
	CreateSprite("UI_8");
	CreateSprite("UI_9");
	CreateSprite("UI_C");
	CreateSprite("UI_D");
	CreateSprite("UI_E");
	CreateSprite("UI_I");
	CreateSprite("UI_L");
	CreateSprite("UI_R");
	CreateSprite("UI_S");
	CreateSprite("UI_T");
	CreateSprite("UI_V");
	CreateSprite("UI_W");
	CreateSprite("UI_Heart_Full");
	CreateSprite("UI_Heart_Half");
	CreateSprite("UI_Heart_Empty");
	CreateSprite("UI_Lives");
	CreateSprite("UI_Coins");
	CreateSprite("UI_X");
	CreateSprite("Slime_1");
	CreateSprite("Slime_2");
	CreateSprite("Slime_Squashed");
	CreateSprite("Slime_Snail_Shell_Hit");
	CreateSprite("Snail_1");
	CreateSprite("Snail_2");
	CreateSprite("Snail_Snail_Shell_Hit");
	CreateSprite("Snail_Shell");
	CreateSprite("Snail_Shell_Crack_1");
	CreateSprite("Snail_Shell_Crack_2");
	CreateSprite("Snail_Shell_Crack_3");
	CreateSprite("Block_Item");
	CreateSprite("Block_Boxed_Coin");
	CreateSprite("Block_Coin");
	CreateSprite("Block_Brick");
	CreateSprite("Block_Crate");
	CreateSprite("Ladder_Top");
	CreateSprite("Ladder_Mid");
	CreateSprite("Arrow_Sign");
	CreateSprite("Bush");
	CreateSprite("Plant");
	CreateSprite("Spikes");
	CreateSprite("Rock");
	CreateSprite("Mushroom1");
	CreateSprite("Mushroom2");
	CreateSprite("Door_Closed_Top");
	CreateSprite("Door_Closed_Mid");
	CreateSprite("Door_Open_Top");
	CreateSprite("Door_Open_Mid");

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

	// Create items for the object poolers
	for (int i = 0; i < 10; ++i)
	{
		m_pickUpPool.emplace_back(GenerateNextEntityID(), Vector2::CENTRE, ePowerUpType::e_Grower, false);
	}

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
	while (!entityLocations.empty())
	{
		const auto location = entityLocations.front();
		entityLocations.pop();

		switch (location.first)
		{
		case eEntityType::e_Coin:
			m_coins.emplace_back(GenerateNextEntityID(), location.second, true);
			break;
		case eEntityType::e_Slime:
			m_slimes.emplace_back(GenerateNextEntityID(), location.second, location.second.y == 768.f ? true : false);
			break;
		case eEntityType::e_Snail:
			m_snails.emplace_back(GenerateNextEntityID(), location.second);
			break;
		default: break;
		}
	}

	// Reset Player
	m_player.Reset();

	m_levelStarted = false;

	m_levelTimer = 200.f;

	return true;
}