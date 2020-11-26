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
	void CheckEntityCollisions(Entity* other) override;
	void SetShouldJump(bool shouldJump);

	e_Direction GetMoveDirectionLimit() const;
	void SetMoveDirectionLimit(e_Direction direction);
	
	EPlayerState GetCurrentPlayerState() const;
	void SetPlayerState(EPlayerState state);

private:
	float m_jumpForce;
	bool m_shouldJumpNextFrame;
	EPlayerState m_currentPlayerState;
	e_Direction m_moveDirectionLimit;

	void Move(float deltaTime) override;
	void Jump(float jumpForce);
	std::string GetTopIdentifier();
	CollisionBoxes GenerateCollisionBoxes() override;
};
