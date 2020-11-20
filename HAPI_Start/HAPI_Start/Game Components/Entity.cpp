#include "Entity.h"
#include "../Graphics/Graphics.h"

Entity::Entity(const Vector2 size, const Vector2 position, const Vector2 velocity, const Vector2 acceleration) :
	m_size(size),
	m_position(position),
	m_velocity(velocity),
	m_acceleration(acceleration) {
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

bool Entity::CheckCollisions(const BoundsRectangle& other) const {
	if (GetGlobalBounds().Overlapping(other)) {
		return true;
	}
	return false;
}

void Entity::AddAnimation(std::vector<std::string>& animation) {
	Animation newAnimation{ animation };
	m_animator.AddAnimation(newAnimation);
}

BoundsRectangle Entity::GetGlobalBounds() const {
	return {
		m_position,
		{ m_position.x + static_cast<float>(m_size.x), m_position.y + static_cast<float>(m_size.y)}
	};
}
