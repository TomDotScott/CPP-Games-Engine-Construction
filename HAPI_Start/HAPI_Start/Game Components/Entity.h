#pragma once
#include <string>
#include "../Utilities/BoundsRectangle.h"
#include "../Utilities/Constants.h"
#include "../Utilities/Vector.h"

class Entity {
public:
	explicit Entity(const std::string& fileName,
		const std::string& textureIdentifier,
		const Vector2 size,
		const Vector2 position = {
			static_cast<float>(constants::k_screenWidth) / 2,
			static_cast<float>(constants::k_screenHeight) / 2
		},
		const Vector2 velocity = {},
		const Vector2 acceleration = {}
	);

	explicit Entity(const std::string& spriteSheetIdentifier, 
		int spriteSheetLocation,
		const Vector2 size,
		const Vector2 position = {
			static_cast<float>(constants::k_screenWidth) / 2,
			static_cast<float>(constants::k_screenHeight) / 2
		},
		const Vector2 velocity = {},
		const Vector2 acceleration = {}
	);

	virtual ~Entity() = default;

	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;

	BoundsRectangle GetGlobalBounds() const;
	Vector2 GetPosition() const;
	void SetPosition(const Vector2 newPos);
	bool CheckCollision(Entity* other);

protected:
	const std::string& m_identifier;
	bool m_hasCollided;
	Vector2 m_size;
	Vector2 m_position;
	Vector2 m_velocity;
	Vector2 m_acceleration;
};