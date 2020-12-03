#include "Game.h"
#include "../Graphics/Graphics.h"
#include <fstream>

Game::Game() :
	PLAYER_WON(false),
	PLAYER_LOST(false),
	m_keyboardData(HAPI.GetKeyboardData()),
	m_controllerData(HAPI.GetControllerData(0)),
	m_tileManager(),
	m_player({ Vector2::CENTRE }),
	m_gameClock(),
	m_gameScore(0),
	m_currentSprite(0),
	m_backgroundPosition(Vector2::ZERO),
	m_backgroundMoveDir(e_Direction::eNone)
{
	if (!Initialise())
	{
		HAPI.UserMessage("An Error Occured", "AN ERROR OCCURED");
		HAPI.Close();
	}
}

void Game::Update()
{
	const float deltaTime = DeltaTime();

	HandleKeyBoardInput();
	HandleControllerInput();

	m_player.Update(deltaTime);

	for (auto& coin : m_coins)
	{
		coin.Update(deltaTime);
		m_player.CheckEntityCollisions(coin);
		coin.CheckEntityCollisions(m_player);
	}

	UpdateEnemies(m_slimes, deltaTime);
	UpdateEnemies(m_snails, deltaTime);

	m_tileManager.CheckPlayerLevelCollisions(m_player);

	// Scroll the background
	if (m_player.GetCurrentDirection() == e_Direction::eRight && m_player.GetMoveDirectionLimit() != e_Direction::eRight)
	{
		m_backgroundPosition.x -= 1.f;
		m_backgroundMoveDir = e_Direction::eLeft;
		if (m_backgroundPosition.x < -constants::k_backgroundTileWidth)
		{
			m_backgroundPosition.x = 0;
		}
	} else if (m_player.GetCurrentDirection() == e_Direction::eLeft && m_player.GetMoveDirectionLimit() != e_Direction::eLeft)
	{
		m_backgroundPosition.x += 1.f;
		m_backgroundMoveDir = e_Direction::eRight;
		if (m_backgroundPosition.x > constants::k_backgroundTileWidth)
		{
			m_backgroundPosition.x = 0;
		}
	}

	// Reset the clock
	m_gameClock = clock();
}

void Game::Render()
{
	Graphics::GetInstance().ClearScreen();

	Graphics::GetInstance().DrawTexture("Background", { m_backgroundPosition.x - constants::k_backgroundTileWidth, 0 });
	Graphics::GetInstance().DrawTexture("Background", { m_backgroundPosition.x - 2 * constants::k_backgroundTileWidth, 0 });
	Graphics::GetInstance().DrawTexture("Background", m_backgroundPosition);
	Graphics::GetInstance().DrawTexture("Background", { m_backgroundPosition.x + constants::k_backgroundTileWidth, 0 });
	Graphics::GetInstance().DrawTexture("Background", { m_backgroundPosition.x + 2 * constants::k_backgroundTileWidth, 0 });

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

	m_player.Render();
}

void Game::CreateSprite(const std::string& spriteSheetIdentifier)
{
	if (!Graphics::GetInstance().CreateSprite(spriteSheetIdentifier, m_currentSprite))
	{
		HAPI.Close();
	}
	m_currentSprite++;
}

float Game::DeltaTime() const
{
	const clock_t programTickCount = clock() - m_gameClock;

	const float ticksToMilliseconds = 1000.f / CLOCKS_PER_SEC;

	return programTickCount * ticksToMilliseconds;
}

bool Game::GetKey(const e_KeyCode keyCode) const
{
	return m_keyboardData.scanCode[static_cast<int>(keyCode)] ? true : false;
}

void Game::HandleKeyBoardInput()
{
	if (GetKey(e_KeyCode::SPACE))
	{
		if (m_player.GetCurrentPlayerState() != e_PlayerState::eJumping)
		{
			m_player.SetShouldJump(true);
		}
	}

	e_Direction playerMoveDir = e_Direction::eNone;
	if (GetKey(e_KeyCode::A) || GetKey(e_KeyCode::LEFT))
	{
		playerMoveDir = e_Direction::eLeft;
	} else if (GetKey(e_KeyCode::D) || GetKey(e_KeyCode::RIGHT))
	{
		playerMoveDir = e_Direction::eRight;
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
			m_player.SetDirection(e_Direction::eRight);
		} else if (leftStickVector.x < 0)
		{
			m_player.SetDirection(e_Direction::eLeft);
		} else
		{
			m_player.SetDirection(e_Direction::eNone);
		}
	}
}

bool Game::Initialise()
{
	if (!Graphics::GetInstance().CreateTexture("Data/PlatformerBackground.tga", "Background"))
	{
		HAPI.UserMessage("Background Could Not Be Loaded", "An Error Occurred");
		return false;
	}
	if (!Graphics::GetInstance().CreateSpriteSheet("Data/GameSpriteSheet.tga"))
	{
		HAPI.UserMessage("Spritesheet Could Not Be Loaded", "An Error Occurred");
		return false;
	}

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
	CreateSprite("Fireball_5");
	CreateSprite("Fireball_6");
	CreateSprite("Fireball_7");
	CreateSprite("Fireball_8");
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
	CreateSprite("Flag_Up_1");
	CreateSprite("Flag_Up_2");
	CreateSprite("Flag_Down");
	CreateSprite("Flag_Pole");
	CreateSprite("Gem_1");
	CreateSprite("Gem_2");
	CreateSprite("Gem_3");
	CreateSprite("Gem_4");
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
	CreateSprite("UI_Heart_Empty");
	CreateSprite("UI_Heart_Half");
	CreateSprite("UI_Heart_Full");
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
	CreateSprite("UI_Key_Not_Found");
	CreateSprite("UI_Key_Found");
	CreateSprite("Key");
	if (!m_tileManager.LoadLevel("./Data/Level1.csv"))
	{
		HAPI.UserMessage("Level Data Could Not Be Loaded", "An Error Occurred");
		return false;
	}

	int currentEntityID{ 0 };

	const auto entityLocations = m_tileManager.GetEntityLocations();

	for (auto& entityLocation : entityLocations)
	{
		switch (entityLocation.first)
		{
		case e_EntityType::eCoin:
			m_coins.emplace_back(currentEntityID, entityLocation.second, true);
			break;
		case e_EntityType::eSlime:
			m_slimes.emplace_back(currentEntityID, entityLocation.second, entityLocation.second.y == 768.f ? true : false);
			break;
		case e_EntityType::eSnail:
			m_snails.emplace_back(currentEntityID, entityLocation.second);
			break;
		default: break;
		}
		currentEntityID++;
	}

	m_player.SetPosition(Vector2::CENTRE);

	return true;
}