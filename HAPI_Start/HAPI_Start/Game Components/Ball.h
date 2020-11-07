#pragma once
#include "Entity.h"
#include "Player.h"

struct Score;

class Ball final : public Entity {
public:
	explicit Ball(const std::string& filename, 
		const std::string& textureIdentifier, 
		const Vector2 position, 
		Vector2 velocity
	);

	explicit Ball(const std::string& spriteSheetIdentifier,
		int spriteSheetLocation,
		const Vector2 position,
		Vector2 velocity
	);
	
	void Update(float deltaTime) override;
	void Render() override;
	
	void SetBallInPlay(bool val);
	bool GetBallInPlay() const;
	
private:
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
