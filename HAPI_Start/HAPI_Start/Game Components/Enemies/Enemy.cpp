#include "Enemy.h"
#include "../Graphics/Graphics.h"

Enemy::Enemy(const Vector2 startingPosition, const Vector2 size, const e_Direction startDir, const bool canAvoidEdges) :
	Entity(
		size,
		startDir,
		startingPosition,
		{ -100.f, 0 }
	),
	m_canAvoidEdges(canAvoidEdges),
	m_isFalling(false)
{
}

void Enemy::Render(const float playerOffset)
{
	if (m_animator.GetCurrentAnimationState() == e_AnimationState::eStarted ||
		m_animator.GetCurrentAnimationState() == e_AnimationState::ePaused)
	{
		Graphics::GetInstance().DrawSprite(
			m_animator.GetCurrentFrameIdentifier(),
			{
				m_position.x + (static_cast<float>(constants::k_screenWidth) / 2.f) - playerOffset,
				m_position.y
			}
		);
	}
}

bool Enemy::CanAvoidEdges() const
{
	return m_canAvoidEdges;
}

void Enemy::SetIsFalling(const bool isFalling)
{
	if (!isFalling)
	{
		m_velocity.y = 0.f;
	}
	m_isFalling = isFalling;
}

void Enemy::CheckSnailShellCollisions(CollisionBoxes& snailShellCollisionBoxes)
{
	if (m_currentCollisionBoxes.m_globalBounds.Overlapping(snailShellCollisionBoxes.m_globalBounds))
	{
		if (m_currentCollisionBoxes.m_leftCollisionBox.Overlapping(snailShellCollisionBoxes.m_rightCollisionBox) ||
			m_currentCollisionBoxes.m_rightCollisionBox.Overlapping(snailShellCollisionBoxes.m_leftCollisionBox))
		{
			m_currentEntityState = e_EntityState::eSnailShellHit;
			m_animator.SetAnimationIndex(static_cast<int>(m_currentEntityState));
		}
	}
}

void Enemy::Move(const float deltaTime)
{
	m_position = m_position + m_velocity * (deltaTime / 1000.f);
}
