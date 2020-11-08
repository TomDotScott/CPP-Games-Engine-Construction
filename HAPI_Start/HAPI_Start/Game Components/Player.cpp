#include "Player.h"

#include "../Graphics/Graphics.h"

Player::Player(const std::string& textureFilename, const std::string& textureIdentifier, const Vector2 startingPosition) :
	Entity(textureFilename,
		textureIdentifier,
		{ constants::k_spriteSheetCellWith * 2, constants::k_spriteSheetCellWith },
		startingPosition,
		{ 1 }
	),
	m_currentDirection(Vector2::ZERO) {
}

Player::Player(const std::string& spriteSheetIdentifier, const Vector2 startingPosition) :
	Entity(spriteSheetIdentifier,
		Vector2(constants::k_spriteSheetCellWith * 2,constants::k_spriteSheetCellWith),
		startingPosition,
		{ 1 }),
	m_currentDirection(Vector2::ZERO) {
}

void Player::Update(const float deltaTime) {
	Move(deltaTime);
}

void Player::SetDirection(const Vector2 direction) {
	m_currentDirection = direction;
}

void Player::Move(const float deltaTime) {
	if (m_currentDirection == Vector2::LEFT) {
		if (m_position.x > constants::k_borderWidth) {
			m_position.x -= m_velocity.x * deltaTime;
		}
	} else if (m_currentDirection == Vector2::RIGHT) {
		if (m_position.x < constants::k_screenWidth - m_size.x - constants::k_borderWidth) {
			m_position.x += m_velocity.x * deltaTime;
		}
	}
	if (m_currentDirection == Vector2::UP) {
		if (m_position.y > constants::k_borderWidth) {
			m_position.y -= m_velocity.y * deltaTime;
		}
	} else if (m_currentDirection == Vector2::DOWN) {
		if (m_position.y < constants::k_screenWidth - m_size.y - constants::k_borderWidth) {
			m_position.y += m_velocity.y * deltaTime;
		}
	}
}
