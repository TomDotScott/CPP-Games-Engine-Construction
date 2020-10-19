#include "Ball.h"

Ball::Ball(const std::string& _filename, const Vector2 _position, const Vector2 _velocity, Entity* _p1, Entity* _p2) :
	Entity(_filename, _position, _velocity),
	m_player1(_p1),
	m_player2(_p2) {
}

void Ball::Update() {
	if (CheckCollision(m_player1) || CheckCollision(m_player2)) {
		m_velocity.x *= -1;
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
