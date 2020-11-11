#pragma once
#include "Entity.h"
#include "Player.h"

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

	int GetLivesRemaining() const;
	void CheckCollisions(BoundsRectangle other, const Vector2 otherPosition);

private:
	float m_speedMultiplier;
	int m_livesRemaining;
	
	void Move(const float deltaTime);
	void Reset();
	
	float HitFactor(Vector2 hitPosition) const;
};