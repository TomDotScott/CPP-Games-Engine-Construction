#include "Player.h"

#include "../Graphics/Graphics.h"

Player::Player(const std::string& spriteSheetIdentifier, const Vector2 startingPosition) :
	Entity(spriteSheetIdentifier,
		Vector2(constants::k_spriteSheetCellWith, constants::k_spriteSheetCellWith * 2),
		startingPosition,
		{ Vector2::ZERO }),
	m_currentDirection(Vector2::ZERO),
	m_jumpForce(4.5f),
	m_isJumping(false),
	m_isGrounded(true) {
}

void Player::Update(const float deltaTime) {
	Move(deltaTime);
	if (m_isGrounded && m_isJumping) {
		Jump();
		m_isGrounded = false;
	}

	m_velocity.y += constants::k_gravity * (deltaTime / 1000);

	m_position = m_position + m_velocity;

	if (m_position.y > constants::k_screenHeight - constants::k_spriteSheetCellWith * 3) {
		m_position.y = constants::k_screenHeight - constants::k_spriteSheetCellWith * 3;
		m_isGrounded = true;
		m_isJumping = false;
	} else if (m_position.y < constants::k_spriteSheetCellWith) {
		m_position.y = constants::k_spriteSheetCellWith;
	}
}

void Player::SetDirection(const Vector2 direction) {
	m_currentDirection = direction;
}

void Player::SetIsJumping(const bool isJumping) {
	m_isJumping = isJumping;
}

Vector2 Player::GetCurrentDirection() const {
	return m_currentDirection;
}

void Player::Move(const float deltaTime) {
	m_position = m_position + (m_currentDirection * deltaTime);
}

void Player::Jump() {
	m_velocity.y = -m_jumpForce;
}
