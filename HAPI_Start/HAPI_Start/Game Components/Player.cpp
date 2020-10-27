#include "Player.h"

Player::Player(const std::string& _textureFileName, const std::string& _textureIdentifier, const Vector2 _startingPosition) :
	Entity(_textureFileName, _textureIdentifier, _startingPosition, { 0, 1 }),
	m_currentDirection(Vector2::ZERO) {
}

void Player::Update() {
	Move();
}

void Player::SetDirection(const Vector2 _direction) {
	m_currentDirection = _direction;
}

void Player::Move() {
	if (m_currentDirection == Vector2::UP) {
		if (m_position.y > constants::k_borderWidth) {
			m_position.y -= m_velocity.y;
		}
	} else if (m_currentDirection == Vector2::DOWN) {
		if (m_position.y < constants::k_screenHeight - m_size.y - constants::k_borderWidth) {
			m_position.y += m_velocity.y;
		}
	}
	SetPosition(m_position);
}
