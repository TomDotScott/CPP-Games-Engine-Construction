#include "Enemy.h"
#include "../Graphics/Graphics.h"

Enemy::Enemy(const Vector2 startingPosition, const bool canAvoidEdges) :
	Entity(
		Vector2(constants::k_spriteSheetCellWidth, constants::k_spriteSheetCellWidth),
		Direction::eLeft,
		startingPosition, { 1, 1 }
	),
	m_currentEnemyState(EEnemyState::eAlive),
	m_canAvoidEdges(canAvoidEdges),
	m_isFalling(false) {

	// Create the Animations
	// Walk animation
	std::vector<std::string> walk{ "Slime_1", "Slime_2" };
	AddAnimation(walk, true, 500.f);
	std::vector<std::string> dead{ "Slime_Squashed" };
	AddAnimation(dead, false, 2000.f);
}

void Enemy::Update(const float deltaTime) {
	if (m_currentEnemyState == EEnemyState::eAlive) {
		Move(deltaTime / 10);

		if (m_isFalling) {
			m_velocity.y += constants::k_gravity * (deltaTime / 1000);
			m_position = m_position + m_velocity;
		}
	}
	m_animator.SetAnimationIndex(static_cast<int>(m_currentEnemyState));
	m_animator.Update(deltaTime);
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

void Enemy::SetEnemyState(const EEnemyState state) {
	m_currentEnemyState = state;
}
