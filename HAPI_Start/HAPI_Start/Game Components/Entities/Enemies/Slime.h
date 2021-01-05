#pragma once
#include "Enemy.h"

/**
 * \brief Slimes are the basic enemies within the demo, they walk in a straight-line,
 * staying on low platforms and falling off higher ones, and get squashed by the player
 * if he jumps on them
 */
class Slime final : public Enemy
{
public:
	explicit       Slime(int entityID, const Vector2& startingPosition, bool canAvoidEdges = false);
	void           Update(float deltaTime) override;
	void           CheckEntityCollisions(Entity& other) override;
	void           Squash() override;

private:
	CollisionBoxes GenerateCollisionBoxes() override;
};
