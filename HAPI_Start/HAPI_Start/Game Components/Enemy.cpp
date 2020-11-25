#include "Enemy.h"
#include "../Graphics/Graphics.h"

Enemy::Enemy(const Vector2 startingPosition, const Vector2 size, const Direction startDir, const bool canAvoidEdges) :
	Entity(
		size,
		startDir,
		startingPosition, 
		{ 1, 1 }
	),
	m_canAvoidEdges(canAvoidEdges),
	m_isFalling(false) {
}

void Enemy::Render(const float playerOffset) {
	if (m_animator.GetCurrentAnimationState() == EAnimationState::eStarted || 
		m_animator.GetCurrentAnimationState() == EAnimationState::ePaused) {
		Graphics::GetInstance().DrawSprite(
			m_animator.GetCurrentFrameIdentifier(),
			{
				m_position.x + (static_cast<float>(constants::k_screenWidth) / 2.f) - playerOffset,
				m_position.y
			}
		);
	}
}

bool Enemy::CanAvoidEdges() const {
	return m_canAvoidEdges;
}

void Enemy::SetIsFalling(const bool isFalling) {
	m_isFalling = isFalling;
}

void Enemy::CheckEntityCollisions(const CollisionBoxes& other) {
	if(m_currentCollisionBoxes.m_globalBounds.Overlapping(other.m_globalBounds)) {
		// If player's feet are on the top of the enemy, squash it
		if(m_currentCollisionBoxes.m_topCollisionBox.Overlapping(other.m_bottomCollisionBox)) {
			m_currentEntityState = EntityState::eDead;
			m_animator.SetAnimationIndex(static_cast<int>(GetCurrentEntityState()));
		}
	}
}

void Enemy::Move(const float deltaTime) {
	if (m_currentDirection == Direction::eRight) {
		m_position = m_position + (Vector2::RIGHT * deltaTime);
	} else if (m_currentDirection == Direction::eLeft) {
		m_position = m_position + (Vector2::LEFT * deltaTime);
	}
}