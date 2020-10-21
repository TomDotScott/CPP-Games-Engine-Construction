#pragma once
#include "Entity.h"
#include "Player.h"

struct Score {
	int player1Score;
	int player2Score;
};

class Ball final : public Entity {
public:
	explicit Ball(const std::string& _filename, const Vector2 _position, Vector2 _velocity, Player& _p1, Player& _p2, Score& _score);
	void Update() override;

private:
	Player& m_player1;
	Player& m_player2;
	Score& m_score;

	void Reset();
	float HitFactor(Vector2 _playerPosition);
};
