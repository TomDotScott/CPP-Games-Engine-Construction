#include "Player.h"

Player::Player(const std::string& _spriteFileName, const Vector2 _startingPosition, const ePlayerNumber _playerNumber) :
	Entity(_spriteFileName, _startingPosition),
	m_playerNumber(_playerNumber),
	m_currentDirection(eDirection::eNone) {
}

void Player::Update() {
	Move();
}

void Player::SetDirection(const eDirection _direction) {
	m_currentDirection = _direction;
}

void Player::Move() {
	switch (m_currentDirection) {
	case eDirection::eNone: break;
	case eDirection::eUp:
		if (m_position.y > 0) {
			m_position.y -= 1;
		}
		break;
	case eDirection::eDown:
		if (m_position.y < constants::k_screenHeight - m_texture->GetSize().y) {
			m_position.y += 1;
		}
		break;
	default:
		break;
	}
	SetPosition(m_position);
}
