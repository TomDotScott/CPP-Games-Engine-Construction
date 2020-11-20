#pragma once
#include "Entity.h"

enum class EPlayerState {
	eIdle, eWalking, eJumping, eClimbing
};

class Player final : public Entity {
public:
	explicit Player(Vector2 startingPosition);

	void Update(float deltaTime) override;
	void Render() override;
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
	std::string GetTopIdentifier();
};
