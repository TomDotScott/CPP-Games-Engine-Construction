#pragma once
#include "Entity.h"
#include "Fireball.h"

/**
 * \brief The current state of the alien.
 * This state is used to determine the movement details of the player and
 * the boss, as well as dictate which animation they should use
 */
enum class eAlienState
{
	e_Idle,
	e_Walking,
	e_Jumping,
	e_Climbing,
	e_Dead
};


/**
 * \brief The Alien class acts as a base class for the aliens int he demo; the
 * player and the boss. These entities have the same behaviours, but are more
 * sophisticated than the basic entity object, with the abilities to jump and shoot
 * fireballs
 */
class Alien : public Entity
{
public:
	Alien(int entityID, eEntityType type, const Vector2& startingPosition, Vector2 size, eDirection startDir);

	void Update(float deltaTime) override = 0;
	void CheckEntityCollisions(Entity& other) override = 0;

	void Render(TextureManager& textureManager) override;
	void Render(TextureManager& textureManager, float playerOffset) override;

	/**
	 * \brief Gets the fireballs connected to the alien
	 * \return A reference to the fireball entities associated with the alien
	 */
	std::vector<Fireball>& GetFireBallPool();

	/**
	 * \brief Gets the current state of the alien
	 * \return The current state held in the variable m_currentAlienState
	 */
	eAlienState GetCurrentAlienState() const;
	
	/**
	 * \brief Sets the current state of the alien
	 * \param state The eAlienState state to change the state of the alien to
	 */
	void        SetAlienState(eAlienState state);

protected:

	// The horizontal speed of the alien
	float m_movementSpeed;
	
	// Details how high the alien can jump
	float m_jumpForce;

	bool  m_canShoot;
	// The cooldown between the shots to prevent spam-fire
	float m_shotCoolDown;

	eAlienState m_currentAlienState;

	// A container for the fireball entities that the alien can shoot
	std::vector<Fireball> m_fireballPool;

	void         Move(float deltaTime) override = 0;
	
	/**
	 * \brief Makes the alien shoot a fireball
	 */
	virtual void Shoot();

	/**
	 * \brief Makes the alien jump
	 * \param jumpForce The "force" to be applied to the jump
	 */
	void Jump(float jumpForce);

	/**
	 * \brief Each alien in is too tall to be represented by one sprite (64x64px)
	 * So the alien entities are made up of 2 sprites, a bottom and a top.
	 * \return The sprite identifier in the TextureManager of the top sprite
	 */
	std::string GetTopIdentifier();

	CollisionBoxes GenerateCollisionBoxes() override = 0;
};
