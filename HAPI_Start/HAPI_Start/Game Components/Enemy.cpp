#include "Enemy.h"
#include "../Graphics/Graphics.h"

Enemy::Enemy(const Vector2 startingPosition, const bool canAvoidEdges) :
	Entity(
		Vector2(constants::k_spriteSheetCellWidth, static_cast<float>(constants::k_spriteSheetCellWidth) / 2.f),
		Direction::eLeft,
		startingPosition, { 1, 1 }
	),
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
	if (m_currentEntityState == EntityState::eAlive) {
		Move(deltaTime / 10);

		if (m_isFalling) {
			m_velocity.y += constants::k_gravity * (deltaTime / 1000);
			m_position = m_position + m_velocity;
		}
	}
	m_animator.SetAnimationIndex(static_cast<int>(m_currentEntityState));
	m_animator.Update(deltaTime);
	m_currentCollisionBoxes = GenerateCollisionBoxes();
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

CollisionBoxes Enemy::GenerateCollisionBoxes() {
	auto entityCollisionBox = BoundsRectangle(Vector2::ZERO, m_size);
	entityCollisionBox.Translate(m_position);

	auto topCollisionBox = BoundsRectangle({ 21.f, 5.f }, { 43.f, 9.f });
	topCollisionBox.Translate(m_position);

	auto bottomCollisionBox = BoundsRectangle({6.f, 28.f}, {58.f, 32.f});
	bottomCollisionBox.Translate(m_position);
	
	auto leftRightCollisionBox = BoundsRectangle({ 6.f, 9.f }, { 32.f, 28.f });
	leftRightCollisionBox.Translate(m_position);

	return{ entityCollisionBox,
		topCollisionBox,
		leftRightCollisionBox,
		leftRightCollisionBox.Translate({26, 0}),
		bottomCollisionBox
	};
}
