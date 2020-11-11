#include "Ball.h"
#include <iostream>

#include "../Graphics/Graphics.h"

Ball::Ball(const std::string& textureFilename, const std::string& textureIdentifier, const Vector2 position, const Vector2 velocity) :
	Entity(textureFilename,
		textureIdentifier,
		Vector2(22, 22),
		position,
		velocity,
		{ 0.1f, 0.1f }),
	m_speedMultiplier(20.f) {
}

Ball::Ball(const std::string& spriteSheetIdentifier, const Vector2 position, const Vector2 velocity) :
	Entity(spriteSheetIdentifier,
		Vector2(constants::k_spriteSheetCellWith, constants::k_spriteSheetCellWith),
		position,
		velocity,
		{ 0.1f, 0.1f }),
	m_speedMultiplier(20.f) {
}

void Ball::Update(const float deltaTime) {
	Move(deltaTime);
}

void Ball::SetBallInPlay(const bool val) {
	// m_isBallInPlay = val;
}

bool Ball::GetBallInPlay() const {
	// return m_isBallInPlay;
	return true;
}

void Ball::Move(const float deltaTime) {

	const BoundsRectangle bounds = GetGlobalBounds();
	if (bounds.TOP_LEFT.x < constants::k_borderWidth || bounds.BOTTOM_RIGHT.x > constants::k_screenWidth - constants::k_borderWidth) {
		m_velocity.x *= -1;
	}
	if (bounds.TOP_LEFT.y < constants::k_borderWidth) {
		m_velocity.y *= -1;
	}

	if (bounds.BOTTOM_RIGHT.y > constants::k_screenHeight + m_size.y * 2) {
		Reset();
	}

	m_position = m_position + (m_velocity * (deltaTime / 1000.f)) * m_speedMultiplier;
}

void Ball::CheckCollisions(const BoundsRectangle other, const Vector2 otherPosition) {
	if (Entity::CheckCollisions(other)) {
		const float y = HitFactor(otherPosition);
		Vector2 direction(1, y);
		direction.Normalised();
		m_velocity = direction * m_speedMultiplier;
	}
}

void Ball::Reset() {
	m_position = { Vector2::CENTRE };
	m_velocity = { 0, m_speedMultiplier };
}

float Ball::HitFactor(const Vector2 playerPosition) const {
	// ||  1 <- at the top of the racket
	// ||
	// ||  0 <- at the middle of the racket
	// ||
	// || -1 <- at the bottom of the racket
	return(m_position.y - playerPosition.y) / m_size.y;
}