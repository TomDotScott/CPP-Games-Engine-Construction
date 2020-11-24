#pragma once
#include <string>
#include "../Utilities/BoundsRectangle.h"
#include "../Utilities/Constants.h"
#include "../Utilities/Vector.h"
#include "Animator.h"

enum class Direction {
	eNone, eLeft, eRight
};

// Every entity will have 4 collision areas as well as
// an overall rectangle called the GlobalBounds
struct CollisionBoxes {
	CollisionBoxes() = default;
	BoundsRectangle m_globalBounds;
	BoundsRectangle m_topCollisionBox;
	BoundsRectangle m_leftCollisionBox;
	BoundsRectangle m_rightCollisionBox;
	BoundsRectangle m_bottomCollisionBox;
};

class Entity {
public:
	explicit Entity(Vector2 size,
		Direction = Direction::eLeft,
		Vector2 position = Vector2::CENTRE,
		Vector2 velocity = {},
		Vector2 acceleration = {}
	);

	virtual ~Entity() = default;

	virtual void Update(float deltaTime) = 0;
	virtual void Render();
	virtual void CheckEntityCollisions(const CollisionBoxes& other) = 0;
	// Different per entity and per animation frame...
	virtual CollisionBoxes GenerateCollisionBoxes() = 0;
	
	BoundsRectangle GetGlobalBounds() const;
	
	Vector2 GetPosition() const;
	void SetPosition(Vector2 newPos);
	
	Vector2 GetVelocity() const;
	void SetVelocity(Vector2 newVel);

	Direction GetCurrentDirection() const;
	void SetDirection(Direction direction);
	
protected:
	Animator m_animator;
	Vector2 m_size;
	Vector2 m_position;
	Vector2 m_velocity;
	Vector2 m_acceleration;
	Direction m_currentDirection;
	CollisionBoxes m_currentCollisionBoxes;

	virtual void Move(float deltaTime);

	void AddAnimation(std::vector<std::string>& animation, bool looping = true, float duration = 100.f);
};
