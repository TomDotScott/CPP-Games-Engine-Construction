#pragma once
#include "../Entity.h"

class Enemy : public Entity
{
public:
	explicit       Enemy(eEntityType type, int      entityID, const Vector2& startingPosition, const Vector2& size,
	               eDirection  startDir, bool canAvoidEdges = false);
	
	void           Render(TextureManager& textureManager, float playerOffset) override;

	void           CheckEntityCollisions(Entity& other) override = 0;
	bool           CanAvoidEdges() const;
	virtual void   CheckSnailShellCollisions(CollisionBoxes& snailShellCollisionBoxes);
	virtual void   Squash() = 0;
	
	void           SetIsFalling(bool isFalling);

protected:
	bool m_canAvoidEdges;
	bool m_isFalling;

	void           Move(float deltaTime) override;
	CollisionBoxes GenerateCollisionBoxes() override = 0;
};
