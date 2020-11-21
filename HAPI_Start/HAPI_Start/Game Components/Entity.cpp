#include "Entity.h"
#include "../Graphics/Graphics.h"

Entity::Entity(const Vector2 size, const Direction direction, const Vector2 position, const Vector2 velocity, const Vector2 acceleration) :
	m_size(size),
	m_position(position),
	m_velocity(velocity),
	m_acceleration(acceleration),
	m_currentDirection(direction) {
}

Vector2 Entity::GetPosition() const {
	return m_position;
}

void Entity::SetPosition(const Vector2 newPos) {
	m_position = newPos;
}

Vector2 Entity::GetVelocity() const {
	return m_velocity;
}

void Entity::SetVelocity(const Vector2 newVel) {
	m_velocity = newVel;
}

Direction Entity::GetCurrentDirection() const {
	return m_currentDirection;
}

void Entity::SetDirection(const Direction direction) {
	m_currentDirection = direction;
}

void Entity::Move(const float deltaTime) {
	if (m_currentDirection == Direction::eRight) {
		m_position = m_position + (Vector2::RIGHT * deltaTime);
	} else if (m_currentDirection == Direction::eLeft) {
		m_position = m_position + (Vector2::LEFT * deltaTime);
	}
}

bool Entity::CheckCollisions(const BoundsRectangle& other) const {
	if (GetGlobalBounds().Overlapping(other)) {
		return true;
	}
	return false;
}

void Entity::AddAnimation(std::vector<std::string>& animation, const bool looping, const float duration) {
	Animation newAnimation{ animation, looping, duration };
	m_animator.AddAnimation(newAnimation);
}

void Entity::Render() {
	Graphics::GetInstance().DrawSprite(
		m_animator.GetCurrentFrameIdentifier(),
		{ static_cast<float>(constants::k_screenWidth) / 2.f, m_position.y
		}
	);
}

BoundsRectangle Entity::GetGlobalBounds() const {
	return {
		m_position,
		{ m_position.x + static_cast<float>(m_size.x), m_position.y + static_cast<float>(m_size.y)}
	};
}
