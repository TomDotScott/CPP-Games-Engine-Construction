#pragma once
#include "Entity.h"

enum class e_PlayerState
{
	eIdle, eWalking, eJumping, eClimbing
};

class Player final : public Entity
{
public:
	explicit Player(Vector2 startingPosition);

	void Update(float deltaTime) override;
	void Render() override;
	void CheckEntityCollisions(Entity* other) override;
	void SetShouldJump(bool shouldJump);

	e_Direction GetMoveDirectionLimit() const;
	void SetMoveDirectionLimit(e_Direction direction);

	e_PlayerState GetCurrentPlayerState() const;
	void SetPlayerState(e_PlayerState state);

private:
	float m_jumpForce;
	bool m_shouldJumpNextFrame;
	e_PlayerState m_currentPlayerState;
	e_Direction m_moveDirectionLimit;

	void Move(float deltaTime) override;
	void Jump(float jumpForce);
	std::string GetTopIdentifier();
	CollisionBoxes GenerateCollisionBoxes() override;
};
