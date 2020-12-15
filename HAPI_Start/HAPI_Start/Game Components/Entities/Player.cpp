#include "Player.h"
#include "../Game.h"
#include "../Graphics/Graphics.h"

Player::Player(const Vector2 startingPosition) :
	Entity(eEntityType::e_Player,
		-1,
		Vector2(constants::k_spriteSheetCellSize, constants::k_spriteSheetCellSize + 13),
		eDirection::e_None,
		startingPosition,
		{ Vector2::ZERO }),
	m_jumpForce(8.f),
	m_shouldJumpNextFrame(false),
	m_canShoot(false),
	m_shotCoolDown(constants::k_fireBallCoolDownTimer),
	m_currentPlayerState(ePlayerState::e_Jumping),
	m_currentPowerUpState(ePowerUpState::e_Normal),
	m_moveDirectionLimit(eDirection::e_None)
{
	m_entityType = eEntityType::e_Player;

	// FOR NORMAL SIZED
	AddAnimation(animations::PLAYER_IDLE, true, 1000.f);
	AddAnimation(animations::PLAYER_WALK);
	AddAnimation(animations::PLAYER_JUMP);
	AddAnimation(animations::PLAYER_CLIMB);

	// FOR FIRE-THROWER POWER-UP
	AddAnimation(animations::PLAYER_POWER_UP_IDLE, true, 1000.f);
	AddAnimation(animations::PLAYER_POWER_UP_WALK);
	AddAnimation(animations::PLAYER_POWER_UP_JUMP);
	AddAnimation(animations::PLAYER_POWER_UP_CLIMB);


	for (int i = 0; i < 5; i++)
	{
		m_fireballPool.emplace_back(Game::GenerateNextEntityId());
	}
}

void Player::Update(const float deltaTime)
{
	Move(deltaTime);

	m_shotCoolDown += deltaTime / 1000.f;

	if (m_canShoot)
	{
		Shoot();
	}

	if (m_currentPlayerState == ePlayerState::e_Walking)
	{
		m_velocity.y = 0;
		if (m_shouldJumpNextFrame)
		{
			Jump(m_jumpForce);
			PlaySFX("Player_Jump");
			m_shouldJumpNextFrame = false;
		}
	} else if (m_currentPlayerState == ePlayerState::e_Jumping)
	{
		m_velocity.y += constants::k_gravity * (deltaTime / 1000);
	}

	m_position = m_position + m_velocity;

	// Can't go off the top of the screen
	if (m_position.y < constants::k_spriteSheetCellSize)
	{
		m_position.y = constants::k_spriteSheetCellSize;
	}

	if (m_currentDirection == eDirection::e_None && m_currentPlayerState != ePlayerState::e_Jumping && !m_shouldJumpNextFrame)
	{
		m_currentPlayerState = ePlayerState::e_Idle;
	}

	SetAnimationIndex(4 * static_cast<int>(m_currentPowerUpState) + static_cast<int>(m_currentPlayerState));
	PlayAnimation(deltaTime);

	// Update the fireballs
	for (auto& ball : m_fireballPool)
	{
		if (ball.GetActiveState())
		{
			ball.Update(deltaTime);
		}
	}
}

void Player::Render()
{
	// Glitches if direction is none so default to facing forward
	const eDirection dir = m_currentDirection == eDirection::e_None ? eDirection::e_Right : m_currentDirection;

	Graphics::GetInstance().DrawSprite(
		GetTopIdentifier(),
		{ static_cast<float>(constants::k_screenWidth) / 2.f, m_position.y - constants::k_spriteSheetCellSize }, dir == eDirection::e_Left
	);

	Graphics::GetInstance().DrawSprite(
		GetCurrentAnimationFrameIdentifier(),
		{ static_cast<float>(constants::k_screenWidth) / 2.f, m_position.y }, dir == eDirection::e_Left
	);
}

void Player::CheckEntityCollisions(Entity& other)
{
	const auto& currentCollisionBoxes = GenerateCollisionBoxes();
	const auto& otherEntColBox = other.GetCurrentCollisionBoxes();
	// Check the global boxes
	if (currentCollisionBoxes.m_globalBounds.Overlapping(otherEntColBox.m_globalBounds))
	{
		switch (other.GetEntityType())
		{
		case eEntityType::e_FireGem:
			// collide from any angle
			if (currentCollisionBoxes.m_leftCollisionBox.Overlapping(otherEntColBox.m_rightCollisionBox) ||
				currentCollisionBoxes.m_rightCollisionBox.Overlapping(otherEntColBox.m_leftCollisionBox) ||
				currentCollisionBoxes.m_topCollisionBox.Overlapping(otherEntColBox.m_bottomCollisionBox) ||
				currentCollisionBoxes.m_bottomCollisionBox.Overlapping(otherEntColBox.m_topCollisionBox))
			{
				PowerUp(ePowerUpType::e_FireThrower);
				PlaySFX("Player_Power_Up");
			}
			break;
		case eEntityType::e_Slime:
		case eEntityType::e_Snail:
			if (other.GetCurrentEntityState() != eEntityState::e_Dead)
			{
				// If touching the bottom...
				if (currentCollisionBoxes.m_bottomCollisionBox.Overlapping(otherEntColBox.m_topCollisionBox))
				{
					// Jump
					Jump(m_jumpForce / 2);
					PlaySFX("Entity_Squash");
					return;
				}

				// If touching the left or right...
				if (currentCollisionBoxes.m_leftCollisionBox.Overlapping(otherEntColBox.m_rightCollisionBox) ||
					currentCollisionBoxes.m_rightCollisionBox.Overlapping(otherEntColBox.m_leftCollisionBox))
				{
					PowerDown();
					PlaySFX("Player_Power_Down");
				}
			}
			break;
		default:;
		}
	}
}

ePlayerState Player::GetCurrentPlayerState() const
{
	return m_currentPlayerState;
}

void Player::SetPlayerState(const ePlayerState state)
{
	m_currentPlayerState = state;
}

ePowerUpState Player::GetPowerUpState() const
{
	return m_currentPowerUpState;
}

void Player::PowerUp(const ePowerUpType pType)
{
	switch (pType)
	{
	case ePowerUpType::e_FireThrower:
		if (m_currentPowerUpState != ePowerUpState::e_FireThrower)
		{
			m_currentPowerUpState = ePowerUpState::e_FireThrower;
		} else
		{
			// TODO: GIVE POINT BONUS IF COLLECTING AN ALREADY EQUIPPED POWER-UP
		}
		break;
	default:;
	}
}

void Player::PowerDown()
{
	switch (m_currentPowerUpState)
	{
	case ePowerUpState::e_Normal:
		// TODO: KILL THE PLAYER
		break;
	case ePowerUpState::e_FireThrower:
		m_currentPowerUpState = ePowerUpState::e_Normal;
		break;
	default:;
	}
}

std::vector<Fireball>& Player::GetFireBallPool()
{
	return m_fireballPool;
}

void Player::SetShouldJump(const bool shouldJump)
{
	m_shouldJumpNextFrame = shouldJump;
}

void Player::SetCanShoot(const bool canShoot)
{
	m_canShoot = canShoot;
}

eDirection Player::GetMoveDirectionLimit() const
{
	return m_moveDirectionLimit;
}

void Player::SetMoveDirectionLimit(const eDirection direction)
{
	m_moveDirectionLimit = direction;
}

void Player::Move(const float deltaTime)
{
	if (m_currentDirection == eDirection::e_Right && m_moveDirectionLimit != eDirection::e_Right)
	{
		m_position = m_position + (Vector2::RIGHT * deltaTime);
	} else if (m_currentDirection == eDirection::e_Left && m_moveDirectionLimit != eDirection::e_Left)
	{
		m_position = m_position + (Vector2::LEFT * deltaTime);
	}
}

void Player::Jump(const float jumpForce)
{
	m_currentPlayerState = ePlayerState::e_Jumping;
	m_velocity.y = -jumpForce;
}

void Player::Shoot()
{
	if (m_shotCoolDown >= constants::k_fireBallCoolDownTimer)
	{
		// Find the first inactive fireball and initialise it 
		for (auto& ball : m_fireballPool)
		{
			if (ball.GetActiveState() == false)
			{
				ball.Initialise(m_position, m_currentDirection);
				m_canShoot = false;
				m_shotCoolDown = 0.f;
				PlaySFX("Fireball_Shoot");
				return;
			}
		}
	}
}

// The animations are just for the body but the character is made up of
// 2 sprites, with one for the head. In order to render the appropriate top,
// we need to edit the string identifier for the sprite to remove the "Bottom"
std::string Player::GetTopIdentifier()
{
	std::string currentFramePlayerTopIdentifier = GetCurrentAnimationFrameIdentifier();

	// Store the frame no to add back to the end
	const auto frameNo = currentFramePlayerTopIdentifier[currentFramePlayerTopIdentifier.size() - 1];
	// Remove the "Body_x"
	currentFramePlayerTopIdentifier.erase(currentFramePlayerTopIdentifier.end() - 6, currentFramePlayerTopIdentifier.end());
	// Add "Top_x"
	currentFramePlayerTopIdentifier.append("Top_");
	return currentFramePlayerTopIdentifier += frameNo;
}

CollisionBoxes Player::GenerateCollisionBoxes()
{
	auto entityCollisionBox = CollisionBox({ 0, 0 }, m_size);
	entityCollisionBox.Translate(m_position);

	auto topBottomCollisionBox = CollisionBox({ 16, 0 }, { 46, 8 });
	topBottomCollisionBox.Translate(m_position);

	auto leftRightCollisionBox = CollisionBox({ 5, 8 }, { 31, 68 });
	leftRightCollisionBox.Translate(m_position);

	return{ entityCollisionBox,
		topBottomCollisionBox,
		leftRightCollisionBox,
		leftRightCollisionBox.Translate({26, 0}),
		topBottomCollisionBox.Translate({0, 68})
	};
}
