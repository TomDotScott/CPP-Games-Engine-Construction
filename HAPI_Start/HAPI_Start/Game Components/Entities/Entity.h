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
	e_Alive, e_Dead, e_ProjectileHit
};

enum class eDirection
{
	e_None = -1, e_Left, e_Right
};

enum class eEntityType
{
	e_None = -1,
	e_Player,
	e_Fireball,
	e_FireGem,
	e_GrowGem,
	e_Slime = 65,
	e_Coin = 66,
	e_Snail = 68,
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
	explicit Entity(eEntityType type,
		int ID,
		Vector2 size,
		eDirection = eDirection::e_Left,
		Vector2 position = Vector2::CENTRE,
		Vector2 velocity = {},
		Vector2 acceleration = {}
	);

	virtual ~Entity() = default;

	virtual void Update(float deltaTime) = 0;
	virtual void Render(TextureManager& textureManager);
	virtual void Render(TextureManager& textureManager, float playerOffset);
	virtual void CheckEntityCollisions(Entity& other) = 0;

	// Different per entity and per animation frame...
	virtual CollisionBoxes GenerateCollisionBoxes() = 0;

	CollisionBoxes GetCurrentCollisionBoxes();

	Vector2 GetPosition() const;
	void SetPosition(Vector2 newPos);

	Vector2 GetVelocity() const;
	void SetVelocity(Vector2 newVel);

	eDirection GetCurrentDirection() const;
	void SetDirection(eDirection direction);

	eEntityState GetCurrentEntityState() const;
	void SetEntityState(eEntityState state);

	eEntityType GetEntityType() const;
	int GetEntityID() const;

protected:
	const int m_entityID;
	std::vector<AnimationPlayer> m_animations;
	int m_animationIndex{ 0 };
	Vector2 m_size;
	Vector2 m_position;
	Vector2 m_velocity;
	Vector2 m_acceleration;
	eDirection m_currentDirection;
	eEntityState m_currentEntityState;
	eEntityType m_entityType;

	virtual void Move(float deltaTime) = 0;
	
	void AddAnimation(const std::vector<std::string>& animationFrameIdentifiers, bool looping = true, float frameLength = 100.f);
	void PlayAnimation(float deltaTime);
	void PlaySFX(const std::string& sfxName);
	void SetAnimationIndex(int animationIndex);
	std::string GetCurrentAnimationFrameIdentifier();
	eAnimationState GetCurrentAnimationState() const;
};
