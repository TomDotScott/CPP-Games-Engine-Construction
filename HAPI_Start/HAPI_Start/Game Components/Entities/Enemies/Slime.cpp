#include "Slime.h"
Slime::Slime(const int entityID, const Vector2 startingPosition, const bool canAvoidEdges) :
	Enemy(e_EntityType::eSlime,
		entityID,
		startingPosition,
		{ static_cast<float>(constants::k_spriteSheetCellWidth), static_cast<float>(constants::k_spriteSheetCellWidth) / 2.f },
		e_Direction::eLeft,
		canAvoidEdges)
{
	m_entityType = e_EntityType::eSlime;
	// Create the Animations
	// Walk animation
	
	AddAnimation(animations::SLIME_WALK, true, 500.f);
	AddAnimation(animations::SLIME_SQUASHED, false, 2000.f);
	AddAnimation(animations::SLIME_SHELL_HIT, false, 1000.f);
}

void Slime::Update(const float deltaTime)
{
	if (m_currentEntityState == e_EntityState::eAlive)
	{
		if (m_isFalling)
		{
			m_velocity.y += constants::k_gravity * (deltaTime / 10.f);
		}
		Move(deltaTime);
	}

	m_animations[m_animationIndex].Play(deltaTime);
	m_currentCollisionBoxes = GenerateCollisionBoxes();
}

void Slime::CheckEntityCollisions(Entity& other)
{
	const auto otherEntColBox = other.GetCurrentCollisionBoxes();
	if (m_currentCollisionBoxes.m_globalBounds.Overlapping(otherEntColBox.m_globalBounds))
	{
		// If player's feet are on the top of the enemy, squash it
		if (m_currentCollisionBoxes.m_topCollisionBox.Overlapping(otherEntColBox.m_bottomCollisionBox))
		{
			m_currentEntityState = e_EntityState::eDead;
			SetAnimationIndex(static_cast<int>(GetCurrentEntityState()));
		}
	}
}

void Slime::Squash()
{
	m_currentEntityState = e_EntityState::eDead;
	SetAnimationIndex(static_cast<int>(GetCurrentEntityState()));
}

CollisionBoxes Slime::GenerateCollisionBoxes()
{
	auto entityCollisionBox = BoundsRectangle(Vector2::ZERO, m_size);
	entityCollisionBox.Translate(m_position);

	auto topCollisionBox = BoundsRectangle({ 21.f, 5.f }, { 43.f, 9.f });
	topCollisionBox.Translate(m_position);

	auto bottomCollisionBox = BoundsRectangle({ 6.f, 28.f }, { 58.f, 32.f });
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
