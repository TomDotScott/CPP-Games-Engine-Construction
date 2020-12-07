#include "Slime.h"
Slime::Slime(const int entityID, const Vector2 startingPosition, const bool canAvoidEdges) :
	Enemy(eEntityType::e_Slime,
		entityID,
		startingPosition,
		{ static_cast<float>(constants::k_spriteSheetCellWidth), static_cast<float>(constants::k_spriteSheetCellWidth) / 2.f },
		eDirection::e_Left,
		canAvoidEdges)
{
	m_entityType = eEntityType::e_Slime;
	// Create the Animations
	// Walk animation
	
	AddAnimation(animations::SLIME_WALK, true, 500.f);
	AddAnimation(animations::SLIME_SQUASHED, false, 2000.f);
	AddAnimation(animations::SLIME_SHELL_HIT, false, 1000.f);
}

void Slime::Update(const float deltaTime)
{
	if (m_currentEntityState == eEntityState::e_Alive)
	{
		if (m_isFalling)
		{
			m_velocity.y += constants::k_gravity * (deltaTime / 10.f);
		}
		Move(deltaTime);
	}

	PlayAnimation(deltaTime);
}

void Slime::CheckEntityCollisions(Entity& other)
{
	const auto& currentCollisionBoxes = GenerateCollisionBoxes();
	const auto& otherEntColBox = other.GetCurrentCollisionBoxes();
	if (currentCollisionBoxes.m_globalBounds.Overlapping(otherEntColBox.m_globalBounds))
	{
		// If player's feet are on the top of the enemy, squash it
		if (currentCollisionBoxes.m_topCollisionBox.Overlapping(otherEntColBox.m_bottomCollisionBox))
		{
			m_currentEntityState = eEntityState::e_Dead;
			SetAnimationIndex(static_cast<int>(GetCurrentEntityState()));
		}
	}
}

void Slime::Squash()
{
	m_currentEntityState = eEntityState::e_Dead;
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
