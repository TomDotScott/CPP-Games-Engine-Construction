#include "Entity.h"
#include "../Graphics/Graphics.h"

Entity::Entity(const std::string& fileName, std::string textureIdentifier, const Vector2 position, const Vector2 velocity, const Vector2 acceleration) :
	m_textureIdentifier(std::move(textureIdentifier)),
	m_size(64, 64),
	m_position(position),
	m_velocity(velocity),
	m_acceleration(acceleration),
	m_physicsClock() {
	if (!Graphics::GetInstance().CreateTexture(fileName, m_textureIdentifier)) {
		const std::string message{ "The file: " + fileName + " was not located. Check the file path or the filename." };
		HAPI.UserMessage(message, "File not found");
	}
}

void Entity::Render() const {
	Graphics::GetInstance().DrawTexture(m_textureIdentifier, m_position);
}

Vector2 Entity::GetPosition() const {
	return m_position;
}

void Entity::SetPosition(const Vector2 newPos) {
	m_position = newPos;
}

bool Entity::CheckCollision(Entity* other) const {
	const auto bounds = this->GetGlobalBounds();
	const auto otherBounds = other->GetGlobalBounds();

	return bounds.m_topLeft.x < otherBounds.m_bottomRight.x&&
		otherBounds.m_topLeft.x < bounds.m_bottomRight.x&&
		bounds.m_topLeft.y < otherBounds.m_bottomRight.y&&
		otherBounds.m_topLeft.y < bounds.m_bottomRight.y;
}

GlobalBounds Entity::GetGlobalBounds() const {
	return {
		m_position,
		{ m_position.x + static_cast<float>(m_size.x), m_position.y },
		{ m_position.x, m_position.y + static_cast<float>(m_size.y)},
		{ m_position.x + static_cast<float>(m_size.x), m_position.y + static_cast<float>(m_size.y)}
	};
}
