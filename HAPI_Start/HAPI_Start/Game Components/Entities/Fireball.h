#pragma once
#include "Entity.h"

/**
 * \brief The state of the fireball, used for determining
 * the movement behaviour of the fireball as well as the
 * correct animation to play
 */
enum class eFireBallState
{
	e_Bouncing,
	e_Exploding
};

/**
 * \brief Fireballs are owned by the Alien class and they are
 * able to be shot out, bouncing along the stage before they hit
 * something. If they hit a tile, they explode, and if they hit
 * another entity, they explode, killing the entity
 */
class Fireball final : public Entity
{
public:
	explicit       Fireball(int entityID);
	void           Update(float deltaTime) override;
	void           CheckEntityCollisions(Entity& other) override;
	CollisionBoxes GenerateCollisionBoxes() override;
	void           Render(TextureManager& textureManager, float playerOffset) override;

	/**
	 * \brief Initialises the fireball, setting it to active and allowing it
	 * to be drawn and updated
	 * \param startPosition The initial position of the fireball
	 * \param startingDirection The movement direction of the fireball
	 */
	void Initialise(Vector2 startPosition, eDirection startingDirection);
	/**
	 * \brief This function is called to bounce the fireball across
	 * the game-world
	 */
	void Bounce();
	/**
	 * \brief This function gets called when the fireball collides with a
	 * tile or entity in the game-world, and triggers the explosion animation
	 */
	void Explode();

private:
	float          m_jumpForce;
	eFireBallState m_fireBallState;
	void           Move(float deltaTime) override;
};
