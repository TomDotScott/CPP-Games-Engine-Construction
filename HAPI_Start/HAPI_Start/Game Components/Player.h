#pragma once
#include "Entity.h"

class Player final : public Entity {
public:
	Player(const std::string& spriteSheetIdentifier,
		Vector2 startingPosition
	);

	void Update(const float deltaTime) override;
	void SetDirection(Vector2 direction);
	void SetIsJumping(const bool isJumping);
	bool GetIsGrounded() const;
	void SetIsGrounded(const bool isGrounded);
	Vector2 GetCurrentDirection() const;

private:
	Vector2 m_currentDirection;
	float m_jumpForce;
	bool m_isJumping;
	bool m_isGrounded;

	void Move(const float deltaTime);
	void Jump();
};
