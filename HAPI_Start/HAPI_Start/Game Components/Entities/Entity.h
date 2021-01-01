#pragma once
#include <string>
#include "../AnimationPlayer.h"
#include "../Animations.h"
#include "../Utilities/CollisionBox.h"
#include "../Utilities/Vector.h"
#include "../Utilities/Constants.h"

class TextureManager;

enum class eEntityState
{
	e_Alive,
	e_Dead,
	e_ProjectileHit
};

enum class eDirection
{
	e_None = -1,
	e_Left,
	e_Right
};

enum class eEntityType
{
	e_None = -1,
	e_Player,
	e_Fireball,
	e_FireGem,
	e_GrowGem,
	e_Flag = 38,
	e_Slime = 65,
	e_Coin = 66,
	e_Snail = 68,
	e_Boss = 73,
	e_Lever = 75
};

// Every entity will have 4 collision areas as well as
// an overall rectangle called the GlobalBounds
struct CollisionBoxes
{
	CollisionBoxes() = default;
	CollisionBox m_globalBounds;
	CollisionBox m_topCollisionBox;
	CollisionBox m_leftCollisionBox;
	CollisionBox m_rightCollisionBox;
	CollisionBox m_bottomCollisionBox;
};

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

	virtual void Update(float deltaTime) = 0;
	virtual void Render(TextureManager& textureManager);
	virtual void Render(TextureManager& textureManager, float playerOffset);
	virtual void CheckEntityCollisions(Entity& other) = 0;

	CollisionBoxes GetCurrentCollisionBoxes();

	Vector2      GetPosition() const;
	Vector2      GetVelocity() const;
	eDirection   GetCurrentDirection() const;
	eEntityState GetCurrentEntityState() const;
	eEntityType  GetEntityType() const;
	int          GetEntityID() const;
	bool         GetActiveState() const;

	void SetPosition(const Vector2& newPos);
	void SetEntityState(eEntityState state);
	void SetDirection(eDirection direction);
	void SetVelocity(const Vector2& newVel);
	void SetActiveState(bool activeState);

protected:
	const int                    m_entityID;
	std::vector<AnimationPlayer> m_animations;
	int                          m_animationIndex{0};
	Vector2                      m_size;
	Vector2                      m_position;
	Vector2                      m_velocity;
	eDirection                   m_currentDirection;
	eEntityState                 m_currentEntityState;
	eEntityType                  m_entityType;
	bool                         m_activeState;

	virtual void Move(float deltaTime) = 0;

	// Different per entity and per animation frame...
	virtual CollisionBoxes GenerateCollisionBoxes() = 0;

	void AddAnimation(const std::vector<std::string>& animationFrameIdentifiers, bool looping = true, float frameLength = 100.f);

	virtual void    PlayAnimation(float deltaTime);
	void            PlaySFX(const std::string& sfxName);
	void            SetAnimationIndex(int animationIndex);
	std::string     GetCurrentAnimationFrameIdentifier();
	eAnimationState GetCurrentAnimationState() const;
};
