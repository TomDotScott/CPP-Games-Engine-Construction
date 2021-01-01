#pragma once
#include "Entity.h"

enum class eFireBallState
{
	e_Bouncing,
	e_Exploding
};

class Fireball final : public Entity
{
public:
	explicit       Fireball(int entityID);
	void           Update(float deltaTime) override;
	void           CheckEntityCollisions(Entity& other) override;
	CollisionBoxes GenerateCollisionBoxes() override;
	void           Render(TextureManager& textureManager, float playerOffset) override;

	void Initialise(Vector2 startPosition, eDirection startingDirection);
	void Bounce();
	void Explode();

private:
	float          m_jumpForce;
	eFireBallState m_fireBallState;
	void           Move(float deltaTime) override;
};
