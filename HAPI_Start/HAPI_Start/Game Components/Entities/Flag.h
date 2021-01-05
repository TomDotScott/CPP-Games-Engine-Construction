#pragma once
#include "Entity.h"

/**
 * \brief The flag is present at the end of each level and gives a points bonus to the player
 * if he is able to touch it
 */
class Flag final : public Entity
{
public:
	Flag(int entityID, const Vector2& startingPosition);

	void           Update(float deltaTime) override;
	CollisionBoxes GenerateCollisionBoxes() override;
	void           CheckEntityCollisions(Entity& other) override;

private:
	void Move(float deltaTime) override;
};
