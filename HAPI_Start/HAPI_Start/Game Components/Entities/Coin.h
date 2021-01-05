#pragma once
#include "Entity.h"

/**
 * \brief Coins are entities that are placed around the world.
 * They disappear when touched by the player, giving him a coin
 * as well as a points bonus for collecting them
 */
class Coin final : public Entity
{
public:
	explicit Coin(int entityID, const Vector2& position, bool visible);
	void     Update(float deltaTime) override;
	void     CheckEntityCollisions(Entity& other) override;

private:
	void           Move(float deltaTime) override;
	CollisionBoxes GenerateCollisionBoxes() override;
};
