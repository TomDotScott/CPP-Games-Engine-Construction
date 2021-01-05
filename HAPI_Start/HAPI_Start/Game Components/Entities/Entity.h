#pragma once
#include <string>
#include "../AnimationPlayer.h"
#include "../Animations.h"
#include "../Utilities/CollisionBox.h"
#include "../Utilities/Vector.h"
#include "../Utilities/Constants.h"

class TextureManager;

/**
 * \brief Every entity has an EntityState, used
 * to determine the behaviours and animations of the
 * entities
 */
enum class eEntityState
{
	e_Alive,
	e_Dead,
	e_ProjectileHit
};


/**
 * \brief Every entity moves based on a direction
 */
enum class eDirection
{
	e_None = -1,
	e_Left,
	e_Right
};

/**
 * \brief The various entity types that are present in the game
 * They each have a numerical value equal to their entries in the
 * level .csv files
 */
enum class eEntityType
{
	e_None = -1,
	e_Player = 0,
	e_Fireball = 1,
	e_FireGem = 2,
	e_GrowGem = 3,
	e_Flag = 38,
	e_Slime = 65,
	e_Coin = 66,
	e_Snail = 68,
	e_Boss = 73,
	e_Lever = 75,
	e_Portal = 78
};

// Every entity will have 4 collision areas as well as
// an overall rectangle called the GlobalBounds
/**
 * \brief Every entity in the game has 4 collision boxes
 * \n m_globalBounds - an overall box that represents the entirety of the entity sprite (usually 64x64px)
 * \n m_topCollisionBox - the box that represents the top of the sprite
 * \n m_leftCollisionBox - the box that represents the left of the sprite
 * \n m_rightCollisionBox - the box that represents the right of the sprite
 * \n m_bottomCollisionBox - the box that represents the bottom of the sprite
 */
struct CollisionBoxes
{
	CollisionBoxes() = default;
	CollisionBox m_globalBounds;
	CollisionBox m_topCollisionBox;
	CollisionBox m_leftCollisionBox;
	CollisionBox m_rightCollisionBox;
	CollisionBox m_bottomCollisionBox;
};

/**
 * \brief The entity class is the base class for everything in the game that
 * can be rendered and interacted with
 */
class Entity
{
public:
	explicit Entity(eEntityType type, int ID, const Vector2& size, eDirection = eDirection::e_Left, const Vector2& position = Vector2::CENTRE, const Vector2& velocity = {}, bool activeState = true);

	// abide to the rule of 5. Entities have a unique ID so cannot be copied 
	Entity(const Entity& other)                = default;
	Entity(Entity&& other) noexcept            = default;
	Entity& operator=(const Entity& other)     = delete;
	Entity& operator=(Entity&& other) noexcept = delete;
	virtual ~Entity()                          = default;

	/**
	 * \brief Update is a function that should get called every frame for the entity.
	 * It determines the behaviours of the entity
	 * \param deltaTime The time difference between Update() calls. This makes the
	 * entities platform independent
	 */
	virtual void Update(float deltaTime) = 0;
	/**
	 * \brief Renders the entity to the screen
	 * \param textureManager The texturemanager holds the sprite and screen information
	 */
	virtual void Render(TextureManager& textureManager);
	/**
	 * \brief Renders the entity to the screen with an offset on the horizontal axis
	 * \param textureManager The texturemanager holds the sprite and screen information
	 * \param xOffset The amount to be offset along the X (horizontal) axis
	 */
	virtual void Render(TextureManager& textureManager, float xOffset);
	/**
	 * \brief Defines the behaviours that will occur when an entity collides with another
	 * \param other the other entity that the current entity is being compared with
	 */
	virtual void CheckEntityCollisions(Entity& other) = 0;

	/**
	 * \brief Gets the current collision boxes of the entity, since the collision boxes
	 * differ with each frame of animation
	 * \return The collisionboxes struct that defines the positions of the various boxes used
	 * for collisions
	 */
	CollisionBoxes GetCurrentCollisionBoxes();

	/**
	 * \brief Gets the current position of the entity
	 * \return the current position of the entity
	 */
	Vector2      GetPosition() const;
	/**
	 * \brief Gets the current velocity of the entity
	 * \return the current velocity of the entity as a Vector2 
	 */
	Vector2      GetVelocity() const;
	/**
	 * \brief Gets the current direction of the entity
	 * \return the current direction of the entity (left, right or none)
	 */
	eDirection   GetCurrentDirection() const;
	/**
	 * \brief Gets the current state of the entity
	 * \return the current state of the entity (alive, dead or hit by a projectile)
	 */
	eEntityState GetCurrentEntityState() const;
	/**
	 * \brief Gets the type of the entity
	 * \return the type that the entity is
	 */
	eEntityType  GetEntityType() const;
	/**
	 * \brief Gets the unique entityID used per entity
	 * \return the entityID of the current entity
	 */
	int          GetEntityID() const;
	/**
	 * \brief Gets whether the entity is currently active within the game
	 * \return True if the entity is active
	 */
	bool         GetActiveState() const;

	/**
	 * \brief Sets the position of the entity
	 * \param newPos the new position of the entity
	 */
	void SetPosition(const Vector2& newPos);
	/**
	 * \brief Changes the current state of the entity
	 * \param state the state that the entity should be set to
	 */
	void SetEntityState(eEntityState state);
	/**
	 * \brief Sets the direction of the entity
	 * \param direction the direction that the entity should be set to move toward
	 */
	void SetDirection(eDirection direction);
	/**
	 * \brief Sets the velocity of the entity
	 * \param newVel the new velocity of the entity as a Vector2
	 */
	void SetVelocity(const Vector2& newVel);
	/**
	 * \brief Sets the current activeState of the entity
	 * \param activeState the activeState to change the entity to
	 */
	void SetActiveState(bool activeState);

protected:
	// the constant, unique, identifier per entity
	const int                    m_entityID;
	// A container of the various animations that each entity can have
	std::vector<AnimationPlayer> m_animations;
	// The current animation of the entity (represented as an index
	// in the m_animations std::vector
	int                          m_animationIndex{0};
	// The size of the entity, used to help generate the collision boxes
	Vector2                      m_size;
	// The current x,y position of the entity in world-space
	Vector2                      m_position;
	// The velocity of the current entity
	Vector2                      m_velocity;
	// The direction (left, right or none) that the entity is traveling in 
	eDirection                   m_currentDirection;
	// The current state of the entity (alive, dead or hit by a projectile) 
	eEntityState                 m_currentEntityState;
	// The type of the entity
	eEntityType                  m_entityType;
	bool                         m_activeState;

	/**
	 * \brief Defines the movement behaviour of the entity
	 * \param deltaTime the time taken between update calls, used for
	 * platform independence
	 */
	virtual void Move(float deltaTime) = 0;

	/**
	 * \brief Generates the current collision boxes of the entity, which are different per entity
	 * and, potentially, per frame of animation of the entity
	 * \return the collisionboxes struct that represents the various collision
	 * boxes of the entity
	 */
	virtual CollisionBoxes GenerateCollisionBoxes() = 0;

	/**
	 * \brief Creates a AnimationPlayer object and adds it to the m_animations std::vector
	 * \param animationFrameIdentifiers The string sprite identifiers of the animation
	 * \param looping True if the animation is a looping animation
	 * \param frameLength The time difference in milliseconds between frames of animation
	 */
	void AddAnimation(const std::vector<std::string>& animationFrameIdentifiers, bool looping = true, float frameLength = 100.f);

	/**
	 * \brief Updates the currently-playing animation
	 * \param deltaTime the time difference between update calls, used to maintain a
	 * constant speed of animation playback
	 */
	virtual void    PlayAnimation(float deltaTime);
	/**
	 * \brief Uses the SoundManager to play a desired sound effect
	 * \param sfxName the sound effect identifier within the SoundManager
	 */
	void            PlaySFX(const std::string& sfxName);
	/**
	 * \brief Sets the index of the m_animations std::vector so that the correct animation
	 * is played
	 * \param animationIndex The index of the animation within the m_animations std::vector
	 */
	void            SetAnimationIndex(int animationIndex);
	/**
	 * \brief Used for rendering, gets the identifier within the TextureManager of the current
	 * frame of animation
	 * \return The string identifier of the current frame of animation
	 */
	std::string     GetCurrentAnimationFrameIdentifier();
	/**
	 * \brief Gets the state (playing, paused or ended) of the currently playing animation
	 * \return The current animation state of the currently playing animation
	 */
	eAnimationState GetCurrentAnimationState() const;
};
