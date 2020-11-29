#pragma once
#include "../Entity.h"

class Enemy : public Entity
{
public:
	explicit Enemy(Vector2 startingPosition, Vector2 size, e_Direction startDir, bool canAvoidEdges = false);
	void Render(float playerOffset);
	bool CanAvoidEdges() const;
	void SetIsFalling(bool isFalling);
	virtual void CheckEntityCollisions(Entity* other) override = 0;
	virtual void CheckSnailShellCollisions(CollisionBoxes& snailShellCollisionBoxes);
	virtual void Squash() = 0;

protected:
	bool m_canAvoidEdges;
	bool m_isFalling;

	void Move(float deltaTime) override;
	virtual CollisionBoxes GenerateCollisionBoxes() override = 0;
};
