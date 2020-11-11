#include "Brick.h"

Brick::Brick(const Vector2 position, const EBrickType type) :
	Entity(type == EBrickType::eRed ? "RedBrick" : "GreenBrick",
		Vector2(constants::k_spriteSheetCellWith, constants::k_spriteSheetCellWith),
		position
	),
	m_explosionAnimationHasStarted(false),
	m_explosionAnimationHasEnded(false),
	m_currentExplosionAnimationFrame(0),
	m_explosionLifetime(600),
	m_isActive(true),
	m_position(position),
	m_type(type) {
}

void Brick::Update(const float deltaTime) {
	if (m_explosionAnimationHasStarted) {
		m_explosionLifetime += deltaTime;
		if (m_explosionLifetime >= 100.0f) {
			m_explosionLifetime = 0.0f;
			if (m_currentExplosionAnimationFrame < 5) {
				++m_currentExplosionAnimationFrame;
			} else {
				m_explosionAnimationHasEnded = true;
			}
		}
	}
}

EBrickType Brick::GetType() const {
	return m_type;
}

bool Brick::GetIsActive() const {
	return m_isActive;
}

bool Brick::GetExplosionAnimationHasStarted() const {
	return m_explosionAnimationHasStarted;
}

bool Brick::GetExplosionAnimationHasEnded() const {
	return m_explosionAnimationHasEnded;
}

int Brick::GetCurrentAnimationFrame() const {
	return m_currentExplosionAnimationFrame;
}

void Brick::CheckCollision(const BoundsRectangle other, int& gameScore) {
	if (Entity::CheckCollisions(other)) {
		if (m_type == EBrickType::eGreen) {
			m_type = EBrickType::eRed;
			gameScore += 32;
		} else {
			m_isActive = false;
			gameScore += 64;
			m_explosionAnimationHasStarted = true;
		}
	}
}
