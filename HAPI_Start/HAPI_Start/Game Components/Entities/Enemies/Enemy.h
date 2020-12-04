#pragma once
#include "../Entity.h"

class Enemy : public Entity
{
public:
	explicit Enemy(eEntityType type, const int entityID, Vector2 startingPosition, Vector2 size, eDirection startDir, bool canAvoidEdges = false);
	void Render(float playerOffset);
	bool CanAvoidEdges() const;
	void SetIsFalling(bool isFalling);
	virtual void CheckEntityCollisions(Entity& other) override = 0;
	virtual void CheckSnailShellCollisions(CollisionBoxes& snailShellCollisionBoxes);
	virtual void Squash() = 0;

protected:
	bool m_canAvoidEdges;
	bool m_isFalling;

	void Move(float deltaTime) override;
	virtual CollisionBoxes GenerateCollisionBoxes() override = 0;
};
