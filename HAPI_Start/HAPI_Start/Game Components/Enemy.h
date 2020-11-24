#pragma once
#include "Entity.h"

class Enemy : public Entity {
public:
	explicit Enemy(Vector2 startingPosition, bool canAvoidEdges = false);
	void Update(float deltaTime) override;
	void Render(float playerOffset);
	bool CanAvoidEdges() const;
	void SetIsFalling(bool isFalling);
	void CheckEntityCollisions(const CollisionBoxes& other) override;
	
private:
	bool m_canAvoidEdges;
	bool m_isFalling;
	void Move(float deltaTime) override;
	CollisionBoxes GenerateCollisionBoxes() override;
};
