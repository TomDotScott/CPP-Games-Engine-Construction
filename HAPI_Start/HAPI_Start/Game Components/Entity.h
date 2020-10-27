#pragma once
#include <ctime>
#include <string>
#include "../Utilities/Constants.h"
#include "../Utilities/Vector.h"

struct GlobalBounds {
	GlobalBounds(const Vector2 topLeft,
		const Vector2 topRight,
		const Vector2 bottomLeft,
		const Vector2 bottomRight) :
		m_topLeft(topLeft),
		m_topRight(topRight),
		m_bottomLeft(bottomLeft),
		m_bottomRight(bottomRight) {
	};
	Vector2 m_topLeft;
	Vector2 m_topRight;
	Vector2 m_bottomLeft;
	Vector2 m_bottomRight;
};

class Entity {
public:
	explicit Entity(const std::string& fileName,
		std::string textureIdentifier,
		Vector2 position = {
			static_cast<float>(constants::k_screenWidth) / 2,
			static_cast<float>(constants::k_screenHeight) / 2
		},
		Vector2 velocity = {},
		Vector2 acceleration = {}
	);

	virtual ~Entity() = default;

	virtual void Update() = 0;
	void Render() const;

	Vector2 GetPosition() const;
	void SetPosition(const Vector2 newPos);
	bool CheckCollision(Entity* other) const;

protected:
	std::string m_textureIdentifier;
	Vector2 m_size;
	Vector2 m_position;
	Vector2 m_velocity;
	Vector2 m_acceleration;
	clock_t m_physicsClock;

	GlobalBounds GetGlobalBounds() const;
};