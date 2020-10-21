#include "Ball.h"

Ball::Ball(const std::string& _filename, const Vector2 _position, const Vector2 _velocity, Player& _p1, Player& _p2) :
	Entity(_filename, _position, _velocity),
	m_player1(_p1),
	m_player2(_p2) {
}

void Ball::Update() {
	if (CheckCollision(&m_player1)) {
		const float y = HitFactor(m_player1.GetPosition());
		Vector2 direction(1, y);
		direction.Normalised();
		m_velocity = direction;
	}
	if (CheckCollision(&m_player2)) {
		const float y = HitFactor(m_player2.GetPosition());
		Vector2 direction(-1, y);
		direction.Normalised();
		m_velocity = direction;
	}


	if (m_position.y < constants::k_borderWidth ||
		m_position.y > constants::k_screenHeight - constants::k_borderWidth - m_texture->GetSize().y) {
		m_velocity.y *= -1;
	}
	//Reset if going off left or right sides
	if (m_position.x < constants::k_borderWidth ||
		m_position.x > constants::k_screenWidth - constants::k_borderWidth - m_texture->GetSize().x) {
		Reset();
	}

	m_position = m_position + m_velocity;
	SetPosition(m_position);
}

void Ball::Reset() {
	SetPosition({ static_cast<float>(constants::k_screenWidth) / 2, static_cast<float>(constants::k_screenHeight) / 2 });
	m_velocity.x = static_cast<float>(constants::rand_range(1, 3));
	if (rand() % 2 == 0) {
		m_velocity.x *= -1;
	}
}

float Ball::HitFactor(Vector2 _playerPosition) {
	// ||  1 <- at the top of the racket
	// ||
	// ||  0 <- at the middle of the racket
	// ||
	// || -1 <- at the bottom of the racket
	return(m_position.y - _playerPosition.y) / m_texture->GetSize().y;
}
