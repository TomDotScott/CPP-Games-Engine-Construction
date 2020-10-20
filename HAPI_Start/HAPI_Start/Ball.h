#pragma once
#include "Entity.h"
#include "Player.h"

class Ball final : public Entity {
public:
	explicit Ball(const std::string& _filename, Vector2 _position, Vector2 _velocity, Player& _p1, Player& _p2);
	void Update() override;

private:
	Player& m_player1;
	Player& m_player2;
	
	void Reset();
	float HitFactor(Vector2 _playerPosition);
};
