#include "PickUpGem.h"
PickUpGem::PickUpGem(const int entityID, const Vector2 position, const bool visible) :
	Entity(
		eEntityType::e_FireGem,
		entityID,
		{ constants::k_spriteSheetCellSize, constants::k_spriteSheetCellSize },
		eDirection::e_None,
		position
	),
	m_activeState(visible)
{
	AddAnimation(animations::GEM_SPIN);
}

void PickUpGem::Update(const float deltaTime)
{
	PlayAnimation(deltaTime);
}

void PickUpGem::CheckEntityCollisions(Entity& other)
{
	// Only collide with the player...
	if (other.GetEntityType() == eEntityType::e_Player)
	{
		const auto& currentCollisionBoxes = GenerateCollisionBoxes();
		if (currentCollisionBoxes.m_globalBounds.Overlapping(other.GetCurrentCollisionBoxes().m_globalBounds))
		{
			if (currentCollisionBoxes.m_topCollisionBox.Overlapping(other.GetCurrentCollisionBoxes().m_bottomCollisionBox) ||
				currentCollisionBoxes.m_leftCollisionBox.Overlapping(other.GetCurrentCollisionBoxes().m_rightCollisionBox) ||
				currentCollisionBoxes.m_rightCollisionBox.Overlapping(other.GetCurrentCollisionBoxes().m_bottomCollisionBox) ||
				currentCollisionBoxes.m_bottomCollisionBox.Overlapping(other.GetCurrentCollisionBoxes().m_topCollisionBox))
			{
				m_activeState = false;
			}
		}
	}
}

CollisionBoxes PickUpGem::GenerateCollisionBoxes()
{
	auto entityCollisionBox = CollisionBox({ 0, 0 }, m_size);
	entityCollisionBox.Translate(m_position);

	auto topBottomCollisionBox = CollisionBox({ 4, 9 }, { 60, 18 });
	topBottomCollisionBox.Translate(m_position);

	auto leftRightCollisionBox = CollisionBox({ 4, 18 }, { 32, 45 });
	leftRightCollisionBox.Translate(m_position);

	return{ entityCollisionBox,
		topBottomCollisionBox,
		leftRightCollisionBox,
		leftRightCollisionBox.Translate({28, 0}),
		topBottomCollisionBox.Translate({0, 36})
	};
}

void PickUpGem::Initialise(Vector2 position)
{
	m_position = position;
	m_activeState = true;
}

void PickUpGem::SetActiveState(const bool visibility)
{
	m_activeState = visibility;
}

bool PickUpGem::GetActiveState() const
{
	return m_activeState;
}

void PickUpGem::Move(float deltaTime)
{
	
}
