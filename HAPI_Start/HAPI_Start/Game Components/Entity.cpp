#include "Entity.h"
#include "../Graphics/Graphics.h"

Entity::Entity(const std::string& _fileName, std::string _textureIdentifier, const Vector2 _position, const Vector2 _velocity, const Vector2 _acceleration) :
	m_textureIdentifier(std::move(_textureIdentifier)),
	m_size(64, 64),
	m_position(_position),
	m_velocity(_velocity),
	m_acceleration(_acceleration),
	m_physicsClock() {
	if (!Graphics::GetInstance().CreateTexture(_fileName, m_textureIdentifier)) {
		const std::string message{ "The file: " + _fileName + " was not located. Check the file path or the filename." };
		HAPI.UserMessage(message, "File not found");
	}
}

void Entity::Render() const {
	Graphics::GetInstance().DrawTexture(m_textureIdentifier, m_position);
}

Vector2 Entity::GetPosition() const {
	return m_position;
}

void Entity::SetPosition(const Vector2 _newPos) {
	m_position = _newPos;
}

bool Entity::CheckCollision(Entity* _other) const {
	const auto bounds = this->GetGlobalBounds();
	const auto otherBounds = _other->GetGlobalBounds();

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
