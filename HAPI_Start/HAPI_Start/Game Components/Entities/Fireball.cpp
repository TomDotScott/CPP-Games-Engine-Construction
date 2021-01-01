#include "Fireball.h"

Fireball::Fireball(const int entityID) :
	Entity(
	       eEntityType::e_Fireball,
	       entityID,
	       Vector2(constants::k_spriteSheetCellSize, constants::k_spriteSheetCellSize)
	      ),
	m_jumpForce(3.f),
	m_fireBallState(eFireBallState::e_Bouncing)
{
	m_activeState = false;
	AddAnimation(animations::FIREBALL_SPIN, true, 150.f);
	AddAnimation(animations::FIREBALL_EXPLOSION, false, 25.f);
}

void Fireball::Initialise(const Vector2 startPosition, const eDirection startingDirection)
{
	m_fireBallState = eFireBallState::e_Bouncing;
	SetAnimationIndex(static_cast<int>(m_fireBallState));
	m_animations[m_animationIndex].ResetAnimation();
	m_position = startPosition;
	m_velocity = (startingDirection == eDirection::e_Right ? Vector2::LEFT : Vector2::RIGHT) + Vector2::DOWN;
	m_velocity.x *= 3.f;
	m_activeState = true;
}

void Fireball::Bounce()
{
	m_velocity.y = 0;
	m_velocity.y = -m_jumpForce;
}

void Fireball::Explode()
{
	if (m_fireBallState != eFireBallState::e_Exploding)
	{
		m_fireBallState = eFireBallState::e_Exploding;
		SetAnimationIndex(static_cast<int>(m_fireBallState));
		m_animations[m_animationIndex].ResetAnimation();
		PlaySFX("Fireball_Explosion");
	}
}

void Fireball::Update(const float deltaTime)
{
	if (m_fireBallState == eFireBallState::e_Bouncing)
	{
		Move(deltaTime);

		// Deactivate if fallen off the bottom of the screen
		if (m_position.y >= constants::k_spriteSheetCellSize * (constants::k_maxTilesVertical + 1))
		{
			m_activeState = false;
		}
	}
	else
	{
		if (GetCurrentAnimationState() == eAnimationState::e_Ended)
		{
			m_activeState = false;
		}
	}
	PlayAnimation(deltaTime);
}

void Fireball::CheckEntityCollisions(Entity& other) {}

CollisionBoxes Fireball::GenerateCollisionBoxes()
{
	auto entityCollisionBox = CollisionBox({0, 0}, m_size);
	entityCollisionBox.Translate(m_position);

	auto topBottomCollisionBox = CollisionBox({21, 20}, {43, 24});
	topBottomCollisionBox.Translate(m_position);

	auto leftRightCollisionBox = CollisionBox({21, 24}, {32, 40});
	leftRightCollisionBox.Translate(m_position);

	return {
		entityCollisionBox,
		topBottomCollisionBox,
		leftRightCollisionBox,
		leftRightCollisionBox.Translate({21, 0}),
		topBottomCollisionBox.Translate({0, 20})
	};
}

void Fireball::Render(TextureManager& textureManager, const float playerOffset)
{
	if (m_activeState)
	{
		Entity::Render(textureManager, playerOffset);
	}
}

void Fireball::Move(const float deltaTime)
{
	m_velocity.y += constants::k_gravity * deltaTime;
	m_position = m_position + m_velocity;
}
