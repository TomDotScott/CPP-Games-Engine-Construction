#pragma once
#include <ctime>
#include <string>
#include "../Utilities/Constants.h"
#include "../Utilities/Vector.h"

struct GlobalBounds {
	GlobalBounds(Vector2 tl, Vector2 tr, Vector2 bl, Vector2 br) :
		m_topLeft(tl),
		m_topRight(tr),
		m_bottomLeft(bl),
		m_bottomRight(br) {
	};
	Vector2 m_topLeft;
	Vector2 m_topRight;
	Vector2 m_bottomLeft;
	Vector2 m_bottomRight;
};

class Entity {
public:
	explicit Entity(const std::string& _fileName,
	                std::string _textureIdentifier,
	                Vector2 _position = {
		                static_cast<float>(constants::k_screenWidth) / 2,
		                static_cast<float>(constants::k_screenHeight) / 2
	                },
	                Vector2 _velocity = {},
	                Vector2 _acceleration = {}
	);

	virtual ~Entity() = default;

	virtual void Update() = 0;
	void Render() const;

	Vector2 GetPosition() const;
	void SetPosition(const Vector2 _newPos);
	bool CheckCollision(Entity* _other) const;

protected:
	std::string m_textureIdentifier;
	Vector2 m_size;
	Vector2 m_position;
	Vector2 m_velocity;
	Vector2 m_acceleration;
	clock_t m_physicsClock;

	GlobalBounds GetGlobalBounds() const;
};