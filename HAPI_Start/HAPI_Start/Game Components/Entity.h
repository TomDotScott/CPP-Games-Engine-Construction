#pragma once
#include <string>
#include "../Utilities/BoundsRectangle.h"
#include "../Utilities/Constants.h"
#include "../Utilities/Vector.h"
#include "Animator.h"

class Entity {
public:
	explicit Entity(Vector2 size,
		Vector2 position = Vector2::CENTRE,
		Vector2 velocity = {},
		Vector2 acceleration = {}
	);

	virtual ~Entity() = default;

	virtual void Update(float deltaTime) = 0;
	virtual void Render();
	
	BoundsRectangle GetGlobalBounds() const;
	
	Vector2 GetPosition() const;
	void SetPosition(Vector2 newPos);
	
	Vector2 GetVelocity() const;
	void SetVelocity(Vector2 newVel);

	Vector2 GetCurrentDirection() const;
	void SetDirection(Vector2 direction);

protected:
	Animator m_animator;
	Vector2 m_size;
	Vector2 m_position;
	Vector2 m_velocity;
	Vector2 m_acceleration;
	Vector2 m_currentDirection;

	virtual void Move(float deltaTime);
	bool CheckCollisions(const BoundsRectangle& other) const;
	void AddAnimation(std::vector<std::string>& animation, float duration = 100.f);
};
