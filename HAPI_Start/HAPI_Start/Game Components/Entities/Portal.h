#pragma once
#include "Entity.h"

/**
 * \brief The Portal is an easter egg entity present in the secret
 * 'Warp-Zone' hidden on level 1
 */
class Portal final : public Entity
{
public:
	Portal(int entityID, const Vector2& position);
	void Update(float deltaTime) override;
	void CheckEntityCollisions(Entity& other) override;

	/**
	 * \brief Gets whether the player should be teleported
	 * out of the warp zone
	 * \return True if the player should be teleported
	 */
	bool GetShouldTeleportPlayer() const;

private:
	bool           m_shouldTeleportPlayer;
	void           Move(float deltaTime) override;
	CollisionBoxes GenerateCollisionBoxes() override;
};
