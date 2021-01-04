#pragma once
#include "Entity.h"

class Portal final : public Entity
{
public:
	Portal(int entityID, const Vector2& position);
	void Update(float deltaTime) override;
	void CheckEntityCollisions(Entity& other) override;

	bool GetShouldTeleportPlayer() const;

private:
	bool           m_shouldTeleportPlayer;
	void           Move(float deltaTime) override;
	CollisionBoxes GenerateCollisionBoxes() override;
};
