#include "Snail.h"
Snail::Snail(const Vector2 startingPos) :
	Enemy(startingPos,
	      Vector2(constants::k_spriteSheetCellWidth, 36),
	      e_Direction::eLeft,
	      true),
	m_speedMultiplier(0.5f),
	m_inShellDuration(0.f),
	m_snailState(e_SnailState::eWalking) {

	m_entityType = e_EntityType::eSnail;
	std::vector<std::string> walk{ "Snail_1", "Snail_2" };
	AddAnimation(walk, true, 500.f);
	std::vector<std::string> squashed{ "Snail_Shell" };
	AddAnimation(squashed, true, 0.1f);
}

void Snail::Update(const float deltaTime) {
	if (m_snailState == e_SnailState::eWalking || m_snailState == e_SnailState::eSliding) {
		Move(deltaTime);
	} else {
		if (m_inShellDuration < 5000.f) {
			m_inShellDuration += deltaTime;
		} else {
			m_snailState = e_SnailState::eWalking;
			m_animator.SetAnimationIndex(static_cast<int>(m_snailState));
		}
	}
	m_animator.Update(deltaTime);
	m_currentCollisionBoxes = GenerateCollisionBoxes();
}

void Snail::CheckEntityCollisions(Entity* other) {
	const auto otherEntColBox = other->GetCurrentCollisionBoxes();
	if (m_currentCollisionBoxes.m_globalBounds.Overlapping(otherEntColBox.m_globalBounds)) {
		if (other->GetEntityType() == e_EntityType::ePlayer) {
			if (m_currentCollisionBoxes.m_topCollisionBox.Overlapping(otherEntColBox.m_bottomCollisionBox)) {
				// If not in shell...
				switch (m_snailState) {
				case e_SnailState::eWalking:
					m_inShellDuration = 0.f;
					m_snailState = e_SnailState::eSquashed;
					m_animator.SetAnimationIndex(static_cast<int>(m_snailState));
					break;
				case e_SnailState::eSquashed:
					m_snailState = e_SnailState::eSliding;
					m_speedMultiplier = 4.f;
					// Work out to go left or right
					if(otherEntColBox.m_bottomCollisionBox.TOP_LEFT.x < m_currentCollisionBoxes.m_rightCollisionBox.TOP_LEFT.x) {
						m_velocity.x = abs(m_velocity.x);
					}else {
						m_velocity.x = -abs(m_velocity.x);
					}
					
					break;
				case e_SnailState::eSliding:
					m_snailState = e_SnailState::eSquashed;
					m_speedMultiplier = 0.5f;
					break;
				default:;
				}
			}
		}
	}
}

e_SnailState Snail::GetSnailState() const {
	return m_snailState;
}

void Snail::Move(const float deltaTime) {
	m_position = m_position + m_velocity * (deltaTime / 1000.f) * m_speedMultiplier;
}

CollisionBoxes Snail::GenerateCollisionBoxes() {
	auto entityCollisionBox = BoundsRectangle(Vector2::ZERO, m_size);
	entityCollisionBox.Translate(m_position);

	auto topCollisionBox = BoundsRectangle({ 22.f, 0.f }, { 52.f, 7.f });
	topCollisionBox.Translate(m_position);

	auto bottomCollisionBox = BoundsRectangle({ 7.f, 30.f }, { 75.f, 36.f });
	bottomCollisionBox.Translate(m_position);

	auto leftRightCollisionBox = BoundsRectangle({ 7.f, 7.f }, { 32.f, 30.f });
	leftRightCollisionBox.Translate(m_position);

	return{ entityCollisionBox,
		topCollisionBox,
		leftRightCollisionBox,
		leftRightCollisionBox.Translate({25.f, 0.f}),
		bottomCollisionBox
	};
}
