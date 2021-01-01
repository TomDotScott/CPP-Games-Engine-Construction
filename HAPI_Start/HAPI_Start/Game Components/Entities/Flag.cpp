#include "Flag.h"

Flag::Flag(const int      entityID,
           const Vector2& startingPosition) :
	Entity(eEntityType::e_Flag,
	       entityID,
	       {constants::k_spriteSheetCellSize, constants::k_spriteSheetCellSize},
	       eDirection::e_None,
	       startingPosition)
{
	AddAnimation(animations::FLAG_WAVE, true, 500.f);
}


void Flag::Update(const float deltaTime)
{
	PlayAnimation(deltaTime);
}

CollisionBoxes Flag::GenerateCollisionBoxes()
{
	auto entityCollisionBox = CollisionBox({0, 0}, m_size);
	entityCollisionBox.Translate(m_position);

	auto topBottomCollisionBox = CollisionBox({0, 0}, m_size);
	topBottomCollisionBox.Translate(m_position);

	auto leftRightCollisionBox = CollisionBox({0, 0}, m_size);
	leftRightCollisionBox.Translate(m_position);

	return {
		entityCollisionBox,
		topBottomCollisionBox,
		leftRightCollisionBox,
		leftRightCollisionBox,
		topBottomCollisionBox
	};
}

void Flag::CheckEntityCollisions(Entity& other) {}

void Flag::Move(float deltaTime) {}
