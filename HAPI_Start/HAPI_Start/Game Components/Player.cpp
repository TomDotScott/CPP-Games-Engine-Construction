#include "Player.h"

#include "../Graphics/Graphics.h"

Player::Player(const std::string& spriteSheetIdentifier, const Vector2 startingPosition) :
	Entity(spriteSheetIdentifier,
		Vector2(constants::k_spriteSheetCellWidth, constants::k_spriteSheetCellWidth * 2),
		startingPosition,
		{ Vector2::ZERO }),
	m_currentDirection(Vector2::ZERO),
	m_jumpForce(4.5f),
	m_isJumping(false),
	m_isGrounded(false) {
}

void Player::Update(const float deltaTime) {
	Move(deltaTime);

	if (m_isGrounded) {
		m_velocity.y = 0;
		if (m_isJumping) {
			Jump();
		}
	} else {
		m_velocity.y += constants::k_gravity * (deltaTime / 1000);
	}

	m_position = m_position + m_velocity;
	// Can't go off the top of the screen
	if (m_position.y < constants::k_spriteSheetCellWidth) {
		m_position.y = constants::k_spriteSheetCellWidth;
	}
}

void Player::SetDirection(const Vector2 direction) {
	m_currentDirection = direction;
}

void Player::SetIsJumping(const bool isJumping) {
	m_isJumping = isJumping;
}

bool Player::GetIsGrounded() const {
	return m_isGrounded;
}

void Player::SetIsGrounded(const bool isGrounded) {
	m_isGrounded = isGrounded;
}

Vector2 Player::GetCurrentDirection() const {
	return m_currentDirection;
}

void Player::Move(const float deltaTime) {
	m_position = m_position + (m_currentDirection * deltaTime);
}

void Player::Jump() {
	m_isGrounded = false;
	m_velocity.y = -m_jumpForce;
}
