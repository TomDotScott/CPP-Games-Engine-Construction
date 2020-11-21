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
	void SetShouldJump(bool shouldJump);

	EPlayerState GetCurrentPlayerState() const;
	void SetPlayerState(EPlayerState state);
	
private:
	float m_jumpForce;
	bool m_shouldJumpNextFrame;
	EPlayerState m_currentState;

	void Jump();
	std::string GetTopIdentifier();
};
