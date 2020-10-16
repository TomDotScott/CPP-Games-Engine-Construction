#pragma once
#include "Constants.h"
#include "Texture.h"
#include "Vector.h"

class Entity
{
public:
	explicit Entity(const std::string& _fileName,
	                Vector2 _position = {constants::k_screenWidth / 2, constants::k_screenHeight / 2},
	                Vector2 _velocity = {0, 0});
	~Entity();
	
	virtual void Update();
	virtual void Render(HAPISPACE::BYTE* _screen);

	bool checkCollision(Entity* _other);
protected:
	Texture* m_texture;
	Vector2 m_position;
	Vector2 m_velocity;
};
