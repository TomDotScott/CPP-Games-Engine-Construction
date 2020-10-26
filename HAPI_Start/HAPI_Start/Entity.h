﻿#pragma once
#include <ctime>
#include "Constants.h"
#include "Texture.h"
#include "Vector.h"


class Entity {
public:
	explicit Entity(const std::string& _fileName,
	                Vector2 _position = { constants::k_screenWidth / 2, constants::k_screenHeight / 2 },
	                Vector2 _velocity = {},
					Vector2 _acceleration = {}
			);
	
	virtual ~Entity();

	virtual void Update() = 0;
	virtual void Render(HAPISPACE::BYTE* _screen) const;
	Vector2 GetPosition() const;
	void SetPosition(const Vector2 _newPos);
	bool CheckCollision(Entity* _other) const;

protected:
	Texture* m_texture;
	Vector2 m_position;
	Vector2 m_velocity;
	Vector2 m_acceleration;
	clock_t m_physicsClock;
};
