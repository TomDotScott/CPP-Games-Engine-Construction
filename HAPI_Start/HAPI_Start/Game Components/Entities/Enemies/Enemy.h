#pragma once
#include "../Entity.h"

/**
 * \brief The Enemy class acts as a parent class for the enemy entities in the game,
 * where they all have the same behaviours, but differ from the entity base class
 */
class Enemy : public Entity
{
public:
	explicit       Enemy(eEntityType type, int      entityID, const Vector2& startingPosition, const Vector2& size,
		eDirection  startDir, bool canAvoidEdges = false);

	/**
	 * \brief Renders the enemy entity to the screen
	 * \param textureManager A reference to the texture manager is needed to draw the sprite data
	 * \param playerOffset The offset of the player along the X (horizontal) axis, to give the illusion of movement
	 */
	void           Render(TextureManager& textureManager, float playerOffset) override;

	void           CheckEntityCollisions(Entity& other) override = 0;
	
	/**
	 * \brief Some enemies can avoid ledges to stay on platforms
	 * \return True if the enemy can stay on the current platform
	 */
	bool           CanAvoidEdges() const;
	
	/**
	 * \brief Enemies interact with snail shells if the player kicks a shell towards them
	 * \param snailShellCollisionBoxes The collision boxes (Overall, top, bottom, left and right) of the shell
	 */
	virtual void   CheckSnailShellCollisions(const CollisionBoxes& snailShellCollisionBoxes);
	
	/**
	 * \brief Enemies have different behaviours if they are jumped on by the player
	 */
	virtual void   Squash() = 0;

	void           SetIsFalling(bool isFalling);

protected:
	bool m_canAvoidEdges;
	// Allows for gravity to be used on the enemies if they aren't on a platform
	bool m_isFalling;

	void           Move(float deltaTime) override;
	CollisionBoxes GenerateCollisionBoxes() override = 0;
};
