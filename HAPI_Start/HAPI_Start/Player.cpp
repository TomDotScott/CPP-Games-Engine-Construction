#include "Player.h"

Player::Player(const std::string& _spriteFileName, const Vector2 _startingPosition, const ePlayerNumber _playerNumber) :
	Entity(_spriteFileName, _startingPosition, { 0, 1 }),
	m_playerNumber(_playerNumber),
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
		if (m_position.y < constants::k_screenHeight - m_texture->GetSize().y - constants::k_borderWidth) {
			m_position.y += m_velocity.y;
		}
	}
	SetPosition(m_position);
}
