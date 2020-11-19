#pragma once
#include <string>
#include "../Utilities/BoundsRectangle.h"
#include "../Utilities/Constants.h"
#include "../Utilities/Vector.h"

class Entity {
public:
	Entity(const std::string& fileName,
		const std::string& textureIdentifier,
		Vector2 size,
		Vector2 position = Vector2::CENTRE,
		Vector2 velocity = {},
		Vector2 acceleration = {}
	);

	Entity(const std::string& spriteSheetIdentifier, 
		Vector2 size,
		Vector2 position = Vector2::CENTRE,
		Vector2 velocity = {},
		Vector2 acceleration = {}
	);

	virtual ~Entity() = default;

	virtual void Update(float deltaTime) = 0;

	BoundsRectangle GetGlobalBounds() const;
	Vector2 GetPosition() const;
	void SetPosition(Vector2 newPos);
	Vector2 GetVelocity() const;
	void SetVelocity(Vector2 newVel);

protected:
	const std::string& m_identifier;
	Vector2 m_size;
	Vector2 m_position;
	Vector2 m_velocity;
	Vector2 m_acceleration;

	bool CheckCollisions(const BoundsRectangle& other) const;
};