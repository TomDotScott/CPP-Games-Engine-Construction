#pragma once
#include "Entity.h"

enum class EPlayerState {
	eOnGround, eJumping
};

class Player final : public Entity {
public:
	Player(const std::string& spriteSheetIdentifier,
		Vector2 startingPosition
	);

	void Update(float deltaTime) override;
	void SetDirection(Vector2 direction);
	void SetShouldJump(bool shouldJump);
	Vector2 GetCurrentDirection() const;

	EPlayerState CURRENT_STATE;
	
private:
	Vector2 m_currentDirection;
	float m_jumpForce;
	bool m_shouldJumpNextFrame;
	
	void Move(float deltaTime);
	void Jump();
};
