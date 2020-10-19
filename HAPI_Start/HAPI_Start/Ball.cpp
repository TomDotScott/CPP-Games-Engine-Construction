#include "Ball.h"

Ball::Ball(const std::string& _filename, const Vector2 _position, const Vector2 _velocity, Entity* _p1, Entity* _p2) :
	Entity(_filename, _position, _velocity),
	m_player1(_p1),
	m_player2(_p2) {
}

void Ball::Update() {
	if (CheckCollision(m_player1)) {
		const float y = HitFactor(m_player1->GetPosition());
		Vector2 direction(1, y);
		direction.Normalised();
		m_velocity = direction;
	}
	if (CheckCollision(m_player2)) {
		const float y = HitFactor(m_player2->GetPosition());
		Vector2 direction(-1, y);
		direction.Normalised();
		m_velocity = direction;
	}


	if (m_position.y < constants::k_borderWidth ||
		m_position.y > constants::k_screenHeight - constants::k_borderWidth - m_texture->GetSize().y) {
		m_velocity.y *= -1;
	}
	// For now, check the x positions
	if (m_position.x < constants::k_borderWidth ||
		m_position.x > constants::k_screenWidth - constants::k_borderWidth - m_texture->GetSize().x) {
		m_velocity.x *= -1;
	}

	m_position = m_position + m_velocity;
	SetPosition(m_position);
}

float Ball::HitFactor(Vector2 _playerPosition) {
	// ||  1 <- at the top of the racket
	// ||
	// ||  0 <- at the middle of the racket
	// ||
	// || -1 <- at the bottom of the racket
	return(m_position.y - _playerPosition.y) / m_texture->GetSize().y;
}
