#include "Portal.h"

Portal::Portal(const int entityID, const Vector2& position) :
	Entity(eEntityType::e_Portal,
		entityID,
		{ constants::k_spriteSheetCellSize, constants::k_spriteSheetCellSize },
		eDirection::e_None,
		position,
		{},
		false),
	m_shouldTeleportPlayer(false)
{
	AddAnimation(animations::PORTAL, true,  75);
}

void Portal::Update(const float deltaTime)
{
	PlayAnimation(deltaTime);
}

void Portal::CheckEntityCollisions(Entity& other)
{
	if (other.GetEntityType() == eEntityType::e_Player)
	{
		const CollisionBoxes currentCollisionBoxes = GenerateCollisionBoxes();
		const CollisionBoxes playerCollisionBoxes = other.GetCurrentCollisionBoxes();

		if (GetCurrentCollisionBoxes().m_globalBounds.Overlapping(other.GetCurrentCollisionBoxes().m_globalBounds))
		{
			if (currentCollisionBoxes.m_leftCollisionBox.Overlapping(playerCollisionBoxes.m_rightCollisionBox) ||
				currentCollisionBoxes.m_rightCollisionBox.Overlapping(playerCollisionBoxes.m_leftCollisionBox) ||
				currentCollisionBoxes.m_topCollisionBox.Overlapping(playerCollisionBoxes.m_bottomCollisionBox) ||
				currentCollisionBoxes.m_bottomCollisionBox.Overlapping(playerCollisionBoxes.m_topCollisionBox))
			{
				m_shouldTeleportPlayer = true;
			}
		}
	}
}

bool Portal::GetShouldTeleportPlayer() const
{
	return m_shouldTeleportPlayer;
}

void           Portal::Move(float deltaTime) {}

CollisionBoxes Portal::GenerateCollisionBoxes()
{
	auto entityCollisionBox = CollisionBox({ 0, 0 }, m_size);
	entityCollisionBox.Translate(m_position);

	auto topBottomCollisionBox = CollisionBox({ 16, 0 }, { 46, 8 });
	topBottomCollisionBox.Translate(m_position);

	auto leftRightCollisionBox = CollisionBox({ 5, 8 }, { 31, 68 });
	leftRightCollisionBox.Translate(m_position);

	return {
		entityCollisionBox,
		topBottomCollisionBox,
		leftRightCollisionBox,
		leftRightCollisionBox.Translate({26, 0}),
		topBottomCollisionBox.Translate({0, 68})
	};
}
