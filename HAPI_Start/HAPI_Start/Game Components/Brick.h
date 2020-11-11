#pragma once
#include "../Utilities/BoundsRectangle.h"
#include "../Utilities/Vector.h"
#include "Entity.h"

enum class EBrickType {
	eRed, eGreen
};

class Brick final : public Entity {
public:
	explicit Brick(Vector2 position, EBrickType type);
	void Update(const float deltaTime) override;

	EBrickType GetType() const;
	bool GetIsActive() const;
	bool GetExplosionAnimationHasStarted() const;
	bool GetExplosionAnimationHasEnded() const;
	int GetCurrentAnimationFrame() const;
	void CheckCollision(const BoundsRectangle other, int& gameScore);
	
private:
	bool m_explosionAnimationHasStarted;
	bool m_explosionAnimationHasEnded;
	int m_currentExplosionAnimationFrame;
	float m_explosionLifetime;
	
	bool m_isActive;
	Vector2 m_position;
	EBrickType m_type;
};