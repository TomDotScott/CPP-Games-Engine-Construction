#pragma once
#include "Enemy.h"

class Slime final : public Enemy
{
public:
	explicit Slime(Vector2 startingPosition, bool canAvoidEdges = false);
	void Update(float deltaTime) override;
	void CheckEntityCollisions(Entity& other) override;
	void Squash() override;
private:
	CollisionBoxes GenerateCollisionBoxes() override;
};
