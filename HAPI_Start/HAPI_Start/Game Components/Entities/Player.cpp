#include "Player.h"
#include "../Game.h"
#include "../Graphics/TextureManager.h"

Player::Player(const Vector2& startingPosition) :
	Alien(Game::GenerateNextEntityID(),
	      eEntityType::e_Player,
	      startingPosition,
	      Vector2(constants::k_spriteSheetCellSize, constants::k_spriteSheetCellSize + 13),
	      eDirection::e_None
	     ),
	m_numStates(5),
	m_shouldJumpNextFrame(false),
	m_currentPowerUpState(ePowerUpState::e_Normal),
	m_moveDirectionLimit(eDirection::e_None),
	m_score(0),
	m_coinCount(0),
	m_livesRemaining(5),
	m_immuneTime(0.f),
	m_immune(false),
	m_isDead(false)
{
	m_entityType = eEntityType::e_Player;

	// FOR SMALL SIZED
	AddAnimation(animations::PLAYER_SMALL_IDLE, true, 1000.f);
	AddAnimation(animations::PLAYER_SMALL_WALK);
	AddAnimation(animations::PLAYER_SMALL_JUMP);
	AddAnimation(animations::PLAYER_SMALL_CLIMB);
	AddAnimation(animations::PLAYER_SMALL_DEAD, false, 1000.f);

	// FOR NORMAL SIZED
	AddAnimation(animations::PLAYER_IDLE, true, 1000.f);
	AddAnimation(animations::PLAYER_WALK);
	AddAnimation(animations::PLAYER_JUMP);
	AddAnimation(animations::PLAYER_CLIMB);
	AddAnimation(animations::PLAYER_DEAD, false, 1000.f);

	// FOR FIRE-THROWER POWER-UP
	AddAnimation(animations::PLAYER_POWER_UP_IDLE, true, 1000.f);
	AddAnimation(animations::PLAYER_POWER_UP_WALK);
	AddAnimation(animations::PLAYER_POWER_UP_JUMP);
	AddAnimation(animations::PLAYER_POWER_UP_CLIMB);
	AddAnimation(animations::PLAYER_POWER_UP_DEAD, false, 1000.f);


	for (int i = 0; i < 5; i++)
	{
		m_fireballPool.emplace_back(Game::GenerateNextEntityID());
	}
}

void Player::Update(const float deltaTime)
{
	if (m_currentAlienState != eAlienState::e_Dead)
	{
		Move(deltaTime);
		m_shotCoolDown += deltaTime;

		if (m_canShoot)
		{
			Shoot();
		}

		if (m_immune)
		{
			m_immuneTime += deltaTime;
			if (m_immuneTime >= 2.f)
			{
				m_immune     = false;
				m_immuneTime = 0.f;
			}
		}

		if (m_currentAlienState == eAlienState::e_Walking)
		{
			m_velocity.y = 0;
			if (m_shouldJumpNextFrame)
			{
				Jump(m_jumpForce);
				PlaySFX("Player_Jump");
				m_shouldJumpNextFrame = false;
			}
		}
		else if (m_currentAlienState == eAlienState::e_Jumping)
		{
			m_velocity.y += constants::k_gravity * deltaTime;
		}

		m_position = m_position + m_velocity;

		// Can't go off the top of the screen
		if (m_position.y < constants::k_spriteSheetCellSize)
		{
			m_position.y = constants::k_spriteSheetCellSize;
		}

		// Can't go past the barrier
		if (m_position.x < constants::k_levelMinX)
		{
			m_position.x = constants::k_levelMinX;
		}

		if (m_currentDirection == eDirection::e_None && m_currentAlienState != eAlienState::e_Jumping && !
		    m_shouldJumpNextFrame)
		{
			m_currentAlienState = eAlienState::e_Idle;
		}

		// Update the fireballs
		for (auto& ball : m_fireballPool)
		{
			if (ball.GetActiveState())
			{
				ball.Update(deltaTime);
			}
		}
	}

	PlayAnimation(deltaTime);

	if (m_currentAlienState == eAlienState::e_Dead && m_animations[m_animationIndex].GetCurrentAnimationState() ==
	    eAnimationState::e_Ended)
	{
		m_isDead = true;
	}
}

void Player::Render(TextureManager& textureManager)
{
	// Glitches if direction is none so default to facing forward
	const eDirection dir = m_currentDirection == eDirection::e_None ? eDirection::e_Right : m_currentDirection;
	if (m_currentPowerUpState != ePowerUpState::e_Small)
	{
		textureManager.DrawSprite(
		                          GetTopIdentifier(),
		                          {
			                          static_cast<float>(constants::k_screenWidth) / 2.f,
			                          m_position.y - constants::k_spriteSheetCellSize
		                          }, dir == eDirection::e_Left,
		                          m_immune && m_currentAlienState != eAlienState::e_Dead ? 100 : 255
		                         );
	}

	textureManager.DrawSprite(
	                          GetCurrentAnimationFrameIdentifier(),
	                          {static_cast<float>(constants::k_screenWidth) / 2.f, m_position.y},
	                          dir == eDirection::e_Left,
	                          m_immune && m_currentAlienState != eAlienState::e_Dead ? 100 : 255
	                         );
}

void Player::PlayAnimation(const float deltaTime)
{
	SetAnimationIndex(m_numStates * static_cast<int>(m_currentPowerUpState) + static_cast<int>(m_currentAlienState));
	Entity::PlayAnimation(deltaTime);
}

ePowerUpState Player::GetPowerUpState() const
{
	return m_currentPowerUpState;
}

void Player::PowerUp(const ePowerUpType pType)
{
	switch (pType)
	{
		case ePowerUpType::e_Grower:
			if (m_currentPowerUpState == ePowerUpState::e_Small)
			{
				m_currentPowerUpState = ePowerUpState::e_Normal;
				m_score += 2000;
				PlaySFX("Player_Power_Up");
			}
			else
			{
				m_score += 1000;
			}
			break;
		case ePowerUpType::e_FireThrower:
			if (m_currentPowerUpState != ePowerUpState::e_FireThrower)
			{
				m_currentPowerUpState = ePowerUpState::e_FireThrower;
				m_score += 2000;
			}
			else
			{
				m_score += 1000;
			}
			break;
	}
	SetAnimationIndex(m_numStates * static_cast<int>(m_currentPowerUpState) + static_cast<int>(m_currentAlienState));
}

void Player::PowerDown()
{
	switch (m_currentPowerUpState)
	{
		case ePowerUpState::e_Small:
			Kill();
			break;
		case ePowerUpState::e_Normal:
			m_currentPowerUpState = ePowerUpState::e_Small;
			PlaySFX("Player_Power_Down");
			break;
		case ePowerUpState::e_FireThrower:
			m_currentPowerUpState = ePowerUpState::e_Normal;
			PlaySFX("Player_Power_Down");
			break;
	}

	m_immune = true;
	SetAnimationIndex(m_numStates * static_cast<int>(m_currentPowerUpState) + static_cast<int>(m_currentAlienState));
}

void Player::AddToScore(const int points)
{
	m_score += points;
}

int Player::GetScore() const
{
	return m_score;
}

void Player::AddCoin()
{
	m_coinCount++;
	if (m_coinCount >= 100)
	{
		m_livesRemaining++;
		m_coinCount = 0;
	}
}

int Player::GetCoinCount() const
{
	return m_coinCount;
}

int Player::GetLivesRemaining() const
{
	return m_livesRemaining;
}

bool Player::GetIsDead() const
{
	return m_isDead;
}

void Player::SetCanShoot(const bool canShoot)
{
	m_canShoot = canShoot;
}

void Player::Kill()
{
	if (m_currentAlienState != eAlienState::e_Dead)
	{
		m_currentAlienState = eAlienState::e_Dead;

		m_livesRemaining--;
		PlaySFX("Player_Dead");

		SetAnimationIndex(m_numStates * static_cast<int>(m_currentPowerUpState) + static_cast<int>(m_currentAlienState
		                  ));
		m_animations[m_animationIndex].ResetAnimation();
	}
}

void Player::Reset(const bool passLevel, const Vector2& startPosition)
{
	m_position         = startPosition;
	m_velocity         = Vector2::ZERO;
	m_currentDirection = eDirection::e_None;

	if (!passLevel)
	{
		m_currentPowerUpState = ePowerUpState::e_Normal;
	}

	m_immune = false;
	m_isDead = false;

	m_currentAlienState = eAlienState::e_Jumping;
	SetAnimationIndex(m_numStates * static_cast<int>(m_currentPowerUpState) + static_cast<int>(m_currentAlienState));
}

void Player::SetShouldJump(const bool shouldJump)
{
	m_shouldJumpNextFrame = shouldJump;
}

eDirection Player::GetMoveDirectionLimit() const
{
	return m_moveDirectionLimit;
}

void Player::SetMoveDirectionLimit(const eDirection direction)
{
	m_moveDirectionLimit = direction;
}

CollisionBoxes Player::GenerateCollisionBoxes()
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

void Player::CheckEntityCollisions(Entity& other)
{
	if (m_currentAlienState != eAlienState::e_Dead)
	{
		const auto& currentCollisionBoxes = GenerateCollisionBoxes();
		const auto& otherEntColBox = other.GetCurrentCollisionBoxes();
		// Check the global boxes
		if (currentCollisionBoxes.m_globalBounds.Overlapping(otherEntColBox.m_globalBounds))
		{
			switch (other.GetEntityType())
			{
			case eEntityType::e_GrowGem:
			case eEntityType::e_FireGem:
				// collide from any angle
				if (currentCollisionBoxes.m_leftCollisionBox.Overlapping(otherEntColBox.m_rightCollisionBox) ||
					currentCollisionBoxes.m_rightCollisionBox.Overlapping(otherEntColBox.m_leftCollisionBox) ||
					currentCollisionBoxes.m_topCollisionBox.Overlapping(otherEntColBox.m_bottomCollisionBox) ||
					currentCollisionBoxes.m_bottomCollisionBox.Overlapping(otherEntColBox.m_topCollisionBox))
				{
					PowerUp(other.GetEntityType() == eEntityType::e_FireGem
						? ePowerUpType::e_FireThrower
						: ePowerUpType::e_Grower);
				}
				break;
			case eEntityType::e_Slime:
				if (other.GetCurrentEntityState() == eEntityState::e_Alive)
				{
					// If touching the bottom...
					if (currentCollisionBoxes.m_bottomCollisionBox.Overlapping(otherEntColBox.m_topCollisionBox))
					{
						// Jump
						Jump(m_jumpForce / 2);
						PlaySFX("Entity_Squash");
						m_score += 200;
						return;
					}
					// If touching the left or right...
					if (currentCollisionBoxes.m_leftCollisionBox.Overlapping(otherEntColBox.m_rightCollisionBox) ||
						currentCollisionBoxes.m_rightCollisionBox.Overlapping(otherEntColBox.m_leftCollisionBox))
					{
						if (!m_immune && !m_isDead)
						{
							PowerDown();
						}
					}
				}
				break;
			case eEntityType::e_Snail:
				if (other.GetCurrentEntityState() == eEntityState::e_Alive)
				{
					// If touching the bottom...
					if (currentCollisionBoxes.m_bottomCollisionBox.Overlapping(otherEntColBox.m_topCollisionBox))
					{
						// Jump
						Jump(m_jumpForce / 2);
						PlaySFX("Entity_Squash");
						m_score += 200;
						return;
					}

					auto* snail = dynamic_cast<Snail*>(&other);

					// Only deal damage to the player if the snail is moving or sliding in its shell
					if (snail->GetSnailState() == eSnailState::e_Sliding || snail->GetSnailState() ==
						eSnailState::e_Walking)
					{
						// If touching the left or right...
						if (currentCollisionBoxes.m_leftCollisionBox.Overlapping(otherEntColBox.m_rightCollisionBox) ||
							currentCollisionBoxes.m_rightCollisionBox.Overlapping(otherEntColBox.m_leftCollisionBox))
						{
							if (!m_immune && !m_isDead)
							{
								PowerDown();
							}
						}
					}
				}
				break;
			case eEntityType::e_Fireball:
			case eEntityType::e_Boss:
				if (currentCollisionBoxes.m_leftCollisionBox.Overlapping(otherEntColBox.m_rightCollisionBox) ||
					currentCollisionBoxes.m_rightCollisionBox.Overlapping(otherEntColBox.m_leftCollisionBox) ||
					currentCollisionBoxes.m_topCollisionBox.Overlapping(otherEntColBox.m_bottomCollisionBox) ||
					currentCollisionBoxes.m_bottomCollisionBox.Overlapping(otherEntColBox.m_topCollisionBox))
				{
					if (!m_immune && !m_isDead)
					{
						PowerDown();
					}
				}
			default:
				break;
			}
		}
	}
}

void Player::Move(const float deltaTime)
{
	if (m_currentDirection == eDirection::e_Right && m_moveDirectionLimit != eDirection::e_Right)
	{
		m_position = m_position + Vector2((m_currentAlienState == eAlienState::e_Jumping ? 0.8f * m_movementSpeed : m_movementSpeed) * 100.f, m_velocity.y) * deltaTime;
	}
	else if (m_currentDirection == eDirection::e_Left && m_moveDirectionLimit != eDirection::e_Left)
	{
		m_position = m_position + Vector2((m_currentAlienState == eAlienState::e_Jumping ? 0.8f * -m_movementSpeed : -m_movementSpeed) * 100.f, m_velocity.y) * deltaTime;
	}
}
