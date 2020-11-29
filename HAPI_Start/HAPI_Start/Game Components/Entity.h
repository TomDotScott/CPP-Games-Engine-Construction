#pragma once
#include <string>
#include "../Utilities/BoundsRectangle.h"
#include "../Utilities/Vector.h"
#include "../Utilities/Constants.h"
#include "Animator.h"

enum class e_EntityState
{
	eAlive, eDead, eSnailShellHit
};

enum class e_Direction
{
	eNone = -1, eLeft, eRight
};

enum class e_EntityType
{
	eNone = -1, ePlayer, eSlime = 65, eCoin = 66, eSnail = 68,
};

// Every entity will have 4 collision areas as well as
// an overall rectangle called the GlobalBounds
struct CollisionBoxes
{
	CollisionBoxes() = default;
	BoundsRectangle m_globalBounds;
	BoundsRectangle m_topCollisionBox;
	BoundsRectangle m_leftCollisionBox;
	BoundsRectangle m_rightCollisionBox;
	BoundsRectangle m_bottomCollisionBox;
};

class Entity
{
public:
	explicit Entity(Vector2 size,
		e_Direction = e_Direction::eLeft,
		Vector2 position = Vector2::CENTRE,
		Vector2 velocity = {},
		Vector2 acceleration = {}
	);

	virtual ~Entity() = default;

	virtual void Update(float deltaTime) = 0;
	virtual void Render();
	virtual void Render(float playerOffset);
	virtual void CheckEntityCollisions(Entity& other) = 0;

	// Different per entity and per animation frame...
	virtual CollisionBoxes GenerateCollisionBoxes() = 0;

	CollisionBoxes GetCurrentCollisionBoxes() const;

	Vector2 GetPosition() const;
	void SetPosition(Vector2 newPos);

	Vector2 GetVelocity() const;
	void SetVelocity(Vector2 newVel);

	e_Direction GetCurrentDirection() const;
	void SetDirection(e_Direction direction);

	e_EntityState GetCurrentEntityState() const;
	void SetEntityState(e_EntityState state);

	e_EntityType GetEntityType() const;

protected:
	Animator m_animator;
	Vector2 m_size;
	Vector2 m_position;
	Vector2 m_velocity;
	Vector2 m_acceleration;
	e_Direction m_currentDirection;
	e_EntityState m_currentEntityState;
	e_EntityType m_entityType;
	CollisionBoxes m_currentCollisionBoxes;

	virtual void Move(float deltaTime) = 0;

	void AddAnimation(std::vector<std::string>& animation, bool looping = true, float duration = 100.f);
};
