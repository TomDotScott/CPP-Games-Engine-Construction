#pragma once
#include "Entity.h"

class Player final : public Entity {
public:
	Player(const std::string& _textureFileName, const std::string& _textureIdentifier, Vector2 _startingPosition);

	void Update() override;
	void SetDirection(Vector2 _direction);

private:
	Vector2 m_currentDirection;
	
	void Move();
};
