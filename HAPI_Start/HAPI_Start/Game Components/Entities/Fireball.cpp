#include "Fireball.h"
Fireball::Fireball(const int entityID) :
	Entity(
		eEntityType::e_Fireball,
		entityID,
		Vector2(constants::k_spriteSheetCellSize, constants::k_spriteSheetCellSize)
	),
	m_activeState(false),
	m_jumpForce(1.5f),
	m_fireBallState(eFireBallState::e_Bouncing)
{
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
	m_velocity.x *= 0.75f;
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

bool Fireball::GetActiveState() const
{
	return m_activeState;
}

void Fireball::Update(const float deltaTime)
{
	if (m_fireBallState == eFireBallState::e_Bouncing)
	{
		Move(deltaTime);
	} else
	{
		if (GetCurrentAnimationState() == eAnimationState::e_Ended)
		{
			m_activeState = false;
		}
	}
	PlayAnimation(deltaTime);
}

void Fireball::CheckEntityCollisions(Entity& other)
{

}

CollisionBoxes Fireball::GenerateCollisionBoxes()
{
	auto entityCollisionBox = CollisionBox({ 0, 0 }, m_size);
	entityCollisionBox.Translate(m_position);

	auto topBottomCollisionBox = CollisionBox({ 21, 20 }, { 43, 24 });
	topBottomCollisionBox.Translate(m_position);

	auto leftRightCollisionBox = CollisionBox({ 21, 24 }, { 32, 40 });
	leftRightCollisionBox.Translate(m_position);

	return{ entityCollisionBox,
		topBottomCollisionBox,
		leftRightCollisionBox,
		leftRightCollisionBox.Translate({21, 0}),
		topBottomCollisionBox.Translate({0, 20})
	};
}

void Fireball::Move(const float deltaTime)
{
	m_velocity.y += constants::k_gravity * (deltaTime / 1000.f);
	m_position = m_position + m_velocity * deltaTime;
}
