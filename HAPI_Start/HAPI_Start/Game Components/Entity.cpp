#include "Entity.h"
#include "../Graphics/Graphics.h"

Entity::Entity(const Vector2 size, const Direction direction, const Vector2 position, const Vector2 velocity, const Vector2 acceleration) :
	m_size(size),
	m_position(position),
	m_velocity(velocity),
	m_acceleration(acceleration),
	m_currentDirection(direction),
	m_currentEntityState(EntityState::eAlive),
	m_currentCollisionBoxes() {

}

CollisionBoxes Entity::GetCurrentCollisionBoxes() const {
	return m_currentCollisionBoxes;
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

EntityState Entity::GetCurrentEntityState() const {
	return m_currentEntityState;
}
void Entity::SetEntityState(const EntityState state) {
	m_currentEntityState = state;
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