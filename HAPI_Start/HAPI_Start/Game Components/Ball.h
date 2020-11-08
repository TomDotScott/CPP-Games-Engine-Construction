#pragma once
#include "Entity.h"
#include "Player.h"

struct Score;

class Ball final : public Entity {
public:
	explicit Ball(const std::string& textureFilename, 
		const std::string& textureIdentifier, 
		const Vector2 position, 
		Vector2 velocity
	);

	explicit Ball(const std::string& spriteSheetIdentifier,
		const Vector2 position,
		Vector2 velocity
	);
	
	void Update(const float deltaTime) override;
	
	void SetBallInPlay(bool val);
	bool GetBallInPlay() const;
	void CheckCollisions(BoundsRectangle other, const Vector2 otherPosition);

private:
	float m_speedMultiplier;
	
	void Move(const float deltaTime);
	void Reset();
	
	float HitFactor(Vector2 playerPosition) const;
};

struct Score {
	int player1Score;
	int player2Score;
};
