#pragma once
#include "Entity.h"

enum class ePlayerNumber {
	ePlayerOne = 1, ePlayerTwo = 2
};

enum class eDirection {
	eNone, eUp, eDown
};

class Player final : public Entity {
public:
	Player(const std::string& _spriteFileName, Vector2 _startingPosition, ePlayerNumber _playerNumber);

	void Update() override;
	void SetDirection(Vector2 _direction);

private:
	ePlayerNumber m_playerNumber;
	Vector2 m_currentDirection;
	
	void Move();
};
