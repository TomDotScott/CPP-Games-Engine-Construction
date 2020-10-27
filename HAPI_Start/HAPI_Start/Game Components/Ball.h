#pragma once
#include "Entity.h"
#include "Player.h"

struct Score;

class Ball final : public Entity {
public:
	explicit Ball(const std::string& filename, 
		const std::string& textureIdentifier, 
		const Vector2 position, 
		Vector2 velocity, 
		Player& p1, 
		Player& p2, 
		Score& score
	);
	
	void Update() override;
	
	void SetBallInPlay(bool _val);
	bool GetBallInPlay() const;
	
private:
	Player& m_player1;
	Player& m_player2;
	Score& m_score;
	bool m_isBallInPlay;

	void Move();
	void Collide();
	void Reset();
	void ScorePlayers();
	void Bounce();
	
	float HitFactor(Vector2 playerPosition) const;
};

struct Score {
	int player1Score;
	int player2Score;
};
