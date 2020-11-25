#pragma once
#include "Entity.h"

class Enemy : public Entity {
public:
	explicit Enemy(Vector2 startingPosition, Vector2 size, Direction startDir, bool canAvoidEdges = false);
	virtual void Update(float deltaTime) override = 0;
	virtual void Render(float playerOffset);
	bool CanAvoidEdges() const;
	void SetIsFalling(bool isFalling);
	void CheckEntityCollisions(const CollisionBoxes& other) override;
	
protected:
	bool m_canAvoidEdges;
	bool m_isFalling;
	
	virtual void Move(float deltaTime) override;
	virtual CollisionBoxes GenerateCollisionBoxes() override = 0;
};
