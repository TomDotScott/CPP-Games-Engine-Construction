#include "Boss.h"
#include "../Game.h"

Boss::Boss(const int      entityID,
           const Vector2& startingPosition,
           const Player&  player) :
	Alien(entityID,
	      eEntityType::e_Boss,
	      startingPosition,
	      Vector2(constants::k_spriteSheetCellSize, constants::k_spriteSheetCellSize + 13),
	      eDirection::e_Left
	     ),
	m_player(player),
	m_battleStarted(false)
{
	AddAnimation(animations::BOSS_IDLE, true, 1000.f);
	AddAnimation(animations::BOSS_WALK, true, 200.f);
	AddAnimation(animations::BOSS_JUMP);
	AddAnimation(animations::BOSS_DEAD, false, 1000.f);

	for (int i = 0; i < 5; i++)
	{
		m_fireballPool.emplace_back(Game::GenerateNextEntityID());
	}

	m_movementSpeed = 100.f;

	m_currentAlienState = eAlienState::e_Idle;
}

void Boss::Update(const float deltaTime)
{
	if (m_position.y < constants::k_screenHeight + constants::k_spriteSheetCellSize)
	{
		const float distanceFromPlayer = m_position.x - m_player.GetPosition().x;
		m_currentDirection             = distanceFromPlayer < 0 ? eDirection::e_Right : eDirection::e_Left;

		// Start the battle if the player is close enough to the boss
		if (distanceFromPlayer < 6 * constants::k_spriteSheetCellSize)
		{
			m_battleStarted = true;
		}

		if (m_battleStarted)
		{
			m_shotCoolDown += deltaTime;

			Move(deltaTime);

			if (m_currentAlienState == eAlienState::e_Walking || m_currentAlienState == eAlienState::e_Idle)
			{
				m_velocity.y = 0;
				if (constants::rand_range(0, 1000) <= 10)
				{
					Jump(m_jumpForce);
				}
				else
				{
					m_currentAlienState = eAlienState::e_Walking;
				}
			}
			else if (m_currentAlienState == eAlienState::e_Jumping)
			{
				m_velocity.y += constants::k_gravity * deltaTime;
				if (constants::rand_range(0, 1000) <= 25)
				{
					Shoot();
				}
			}

			m_position = m_position + m_velocity;
		}

		SetAnimationIndex(static_cast<int>(m_currentAlienState));
		PlayAnimation(deltaTime);
	}

	// Update the fireballs
	for (auto& ball : m_fireballPool)
	{
		ball.Update(deltaTime);
	}
}

void Boss::CheckEntityCollisions(Entity& other)
{
	const auto& currentCollisionBoxes = GenerateCollisionBoxes();
	const auto& otherEntColBox        = other.GetCurrentCollisionBoxes();
	// Check the global boxes
	if (currentCollisionBoxes.m_globalBounds.Overlapping(otherEntColBox.m_globalBounds))
	{
		// TODO : MAKE THE BOSS VULNERABLE TO THE PLAYER'S FIREBALLS
	}
}

void Boss::Render(TextureManager& textureManager, const float playerOffset)
{
	// To save code duplication, call the parent function
	if (m_activeState)
	{
		Alien::Render(textureManager, playerOffset);

		for (auto& fireball : m_fireballPool)
		{
			fireball.Render(textureManager, playerOffset);
		}
	}
}

bool Boss::GetBattleStarted() const
{
	return m_battleStarted;
}

void Boss::SetBattleStarted(const bool battleStarted)
{
	m_battleStarted = battleStarted;
}

CollisionBoxes Boss::GenerateCollisionBoxes()
{
	auto entityCollisionBox = CollisionBox({0, 0}, m_size);
	entityCollisionBox.Translate(m_position);

	auto topBottomCollisionBox = CollisionBox({16, 0}, {46, 8});
	topBottomCollisionBox.Translate(m_position);

	auto leftRightCollisionBox = CollisionBox({5, 8}, {31, 68});
	leftRightCollisionBox.Translate(m_position);

	return {
		entityCollisionBox,
		topBottomCollisionBox,
		leftRightCollisionBox,
		leftRightCollisionBox.Translate({26, 0}),
		topBottomCollisionBox.Translate({0, 68})
	};
}

void Boss::Move(const float deltaTime)
{
	if (m_currentDirection == eDirection::e_Right)
	{
		m_position = m_position + Vector2((m_currentAlienState == eAlienState::e_Jumping
			                                   ? 0.8f * m_movementSpeed
			                                   : m_movementSpeed), m_velocity.y) * deltaTime;
	}
	else
	{
		m_position = m_position + Vector2((m_currentAlienState == eAlienState::e_Jumping
			                                   ? 0.8f * -m_movementSpeed
			                                   : -m_movementSpeed), m_velocity.y) * deltaTime;
	}

	if (m_position.x < constants::k_bossMinX)
	{
		m_position.x = constants::k_bossMinX;
	}

	if (m_position.x > constants::k_bossMaxX)
	{
		m_position.x = constants::k_bossMaxX;
	}
}

void Boss::Shoot()
{
	if (m_shotCoolDown >= constants::k_fireBallCoolDownTimer)
	{
		// Find the first inactive fireball and initialise it 
		for (auto& ball : m_fireballPool)
		{
			if (ball.GetActiveState() == false)
			{
				if (m_currentDirection == eDirection::e_Left)
				{
					ball.Initialise(m_position, eDirection::e_Right);
				}
				else
				{
					ball.Initialise(m_position, eDirection::e_Left);
				}
				m_canShoot     = false;
				m_shotCoolDown = 0.f;
				PlaySFX("Fireball_Shoot");
				return;
			}
		}
	}
}
