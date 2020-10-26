#include "Ball.h"

Ball::Ball(const std::string& _filename, const Vector2 _position, const Vector2 _velocity, Player& _p1,
	Player& _p2, Score& _score) :
	Entity(_filename, _position, _velocity, {0.1f, 0.1f}),
	m_player1(_p1),
	m_player2(_p2),
	m_score(_score),
	m_isBallInPlay(false) {
}

void Ball::Update() {
	if (m_isBallInPlay) {
		Bounce();
		Collide();
		ScorePlayers();
		Move();
	}
}

void Ball::SetBallInPlay(const bool _val) {
	m_isBallInPlay = _val;
}

bool Ball::GetBallInPlay() const {
	return m_isBallInPlay;
}

void Ball::Move() {
	SetPosition(m_position + m_velocity);
}

void Ball::Collide() {
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

	m_velocity.Limit({ m_velocity.x, 3.f });
}

void Ball::Reset() {
	SetPosition({ static_cast<float>(constants::k_screenWidth) / 2, static_cast<float>(constants::k_screenHeight) / 2 });
	m_isBallInPlay = false;
	m_velocity = { 1, 0 };
	if (rand() % 2 == 0) {
		m_velocity.x *= -1;
	}
}

void Ball::ScorePlayers() {
	//Reset if going off left or right sides
	if (m_position.x < constants::k_borderWidth) {
		m_score.player2Score += 1;
		Reset();
	}
	if (m_position.x > constants::k_screenWidth - constants::k_borderWidth - m_texture->GetSize().x) {
		m_score.player1Score += 1;
		Reset();
	}
}

void Ball::Bounce() {
	if (m_position.y < constants::k_borderWidth ||
		m_position.y > constants::k_screenHeight - constants::k_borderWidth - m_texture->GetSize().y) {
		m_velocity.y *= -1;
	}
}

float Ball::HitFactor(Vector2 _playerPosition) const {
	// ||  1 <- at the top of the racket
	// ||
	// ||  0 <- at the middle of the racket
	// ||
	// || -1 <- at the bottom of the racket
	return(m_position.y - _playerPosition.y) / m_texture->GetSize().y;
}
