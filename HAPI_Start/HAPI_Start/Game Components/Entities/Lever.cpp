#include "Lever.h"

Lever::Lever(const int      entityID,
             const Vector2& startPosition) :
	Entity(eEntityType::e_Lever,
	       entityID,
	       {constants::k_spriteSheetCellSize, constants::k_spriteSheetCellSize},
	       eDirection::e_None,
	       startPosition),
	m_active(false),
	m_leverState(eLeverState::e_Static)
{
	AddAnimation(animations::LEVER);
	AddAnimation(animations::LEVER_SWING, false, 100);
}

void Lever::Update(const float deltaTime)
{
	SetAnimationIndex(static_cast<int>(m_leverState));
	PlayAnimation(deltaTime);
}

void Lever::CheckEntityCollisions(Entity& other)
{
	if (m_leverState != eLeverState::e_Swinging)
	{
		const auto leverCollisionBoxes = GenerateCollisionBoxes();
		const auto otherCollisionBoxes = other.GetCurrentCollisionBoxes();
		if (leverCollisionBoxes.m_globalBounds.Overlapping(otherCollisionBoxes.m_globalBounds) && 
			other.GetEntityType() == eEntityType::e_Player)
		{
			if (leverCollisionBoxes.m_leftCollisionBox.Overlapping(otherCollisionBoxes.m_rightCollisionBox) ||
			    leverCollisionBoxes.m_rightCollisionBox.Overlapping(otherCollisionBoxes.m_leftCollisionBox) ||
			    leverCollisionBoxes.m_topCollisionBox.Overlapping(otherCollisionBoxes.m_bottomCollisionBox))
			{
				m_leverState = eLeverState::e_Swinging;
			}
		}
	}
}

bool Lever::GetIsActive() const
{
	return m_active;
}

eLeverState Lever::GetLeverState() const
{
	return m_leverState;
}

void Lever::SetActive(const bool active)
{
	m_active = active;
}

CollisionBoxes Lever::GenerateCollisionBoxes()
{
	auto entityCollisionBox = CollisionBox({0, 0}, m_size);
	entityCollisionBox.Translate(m_position);

	auto topBottomCollisionBox = CollisionBox({16, 36}, {48, 43});
	topBottomCollisionBox.Translate(m_position);

	auto leftRightCollisionBox = CollisionBox({16, 43}, {32, 57});
	leftRightCollisionBox.Translate(m_position);

	return {
		entityCollisionBox,
		topBottomCollisionBox,
		leftRightCollisionBox,
		leftRightCollisionBox.Translate({16, 0}),
		topBottomCollisionBox.Translate({0, 21})
	};
}

void Lever::Move(float deltaTime) {}
