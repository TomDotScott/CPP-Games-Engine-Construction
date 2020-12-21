#pragma once
#include "Entity.h"
class Flag final : public Entity
{
public:
	explicit Flag(const int entityID, Vector2 startingPosition);

	void Update(float deltaTime) override;
	CollisionBoxes GenerateCollisionBoxes() override;
	void CheckEntityCollisions(Entity& other) override;
private:
	void Move(float deltaTime) override;
};