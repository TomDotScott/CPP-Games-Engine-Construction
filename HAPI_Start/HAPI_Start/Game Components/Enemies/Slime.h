#pragma once
#include "Enemy.h"

class Slime final : public Enemy
{
public:
	explicit Slime(const int entityID, Vector2 startingPosition, bool canAvoidEdges = false);
	void Update(float deltaTime) override;
	void CheckEntityCollisions(Entity& other) override;
	void Squash() override;
private:
	CollisionBoxes GenerateCollisionBoxes() override;
};
