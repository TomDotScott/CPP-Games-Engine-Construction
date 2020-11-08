#pragma once
#include <string>
#include "../Utilities/BoundsRectangle.h"
#include "../Utilities/Constants.h"
#include "../Utilities/Vector.h"

class Entity {
public:
	Entity(const std::string& fileName,
		const std::string& textureIdentifier,
		const Vector2 size,
		const Vector2 position = Vector2::CENTRE,
		const Vector2 velocity = {},
		const Vector2 acceleration = {}
	);

	Entity(const std::string& spriteSheetIdentifier, 
		const Vector2 size,
		const Vector2 position = Vector2::CENTRE,
		const Vector2 velocity = {},
		const Vector2 acceleration = {}
	);

	virtual ~Entity() = default;

	virtual void Update(float deltaTime) = 0;

	BoundsRectangle GetGlobalBounds() const;
	Vector2 GetPosition() const;
	void SetPosition(const Vector2 newPos);

protected:
	const std::string& m_identifier;
	Vector2 m_size;
	Vector2 m_position;
	Vector2 m_velocity;
	Vector2 m_acceleration;

	bool CheckCollisions(BoundsRectangle other) const;
};