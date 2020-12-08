#pragma once
#include "Entity.h"
enum class eFireBallState
{
	e_Bouncing,
	e_Exploding
};

class Fireball : public Entity
{
public:
	Fireball(int entityID);
	void Update(float deltaTime) override;
	void CheckEntityCollisions(Entity& other) override;
	CollisionBoxes GenerateCollisionBoxes() override;

	void Initialise(Vector2 startPosition, eDirection startingDirection);
	void Bounce();
	void Explode();

	bool GetActiveState() const;

private:
	bool m_activeState;
	float m_jumpForce;
	eFireBallState m_fireBallState;
	void Move(float deltaTime) override;
};

