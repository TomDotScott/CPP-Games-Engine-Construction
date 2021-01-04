#pragma once
#include "Entity.h"

enum class eLeverState
{
	e_Static,
	e_Swinging
};

class Lever final : public Entity
{
public:
	Lever(int entityID, const Vector2& startPosition);
	void Update(float deltaTime) override;
	void CheckEntityCollisions(Entity& other) override;

	eLeverState GetLeverState() const;

private:
	eLeverState m_leverState;

	void           Move(float deltaTime) override;
	CollisionBoxes GenerateCollisionBoxes() override;
};
