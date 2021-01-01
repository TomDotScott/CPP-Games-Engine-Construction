#pragma once
#include "Entity.h"

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
