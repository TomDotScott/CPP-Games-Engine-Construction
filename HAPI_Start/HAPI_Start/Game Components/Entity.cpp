#include "Entity.h"
#include "../Graphics/Graphics.h"

Entity::Entity(const std::string& fileName, const std::string& textureIdentifier, const Vector2 size, const Vector2 position, const Vector2 velocity, const Vector2 acceleration) :
	m_identifier(textureIdentifier),
	m_hasCollided(false),
	m_size(size),
	m_position(position),
	m_velocity(velocity),
	m_acceleration(acceleration) {
	if (!Graphics::GetInstance().CreateTexture(fileName, m_identifier)) {
		const std::string message{ "The file: " + fileName + " was not located. Check the file path or the filename." };
		HAPI.UserMessage(message, "File not found");
	}
}

Entity::Entity(const std::string& spriteSheetIdentifier, const int spriteSheetLocation, const Vector2 size, const Vector2 position, const Vector2 velocity, const Vector2 acceleration) :
	m_identifier(spriteSheetIdentifier),
	m_hasCollided(false),
	m_size(size),
	m_position(position),
	m_velocity(velocity),
	m_acceleration(acceleration) {
	Graphics::GetInstance().CreateSprite(spriteSheetIdentifier, spriteSheetLocation);
}

Vector2 Entity::GetPosition() const {
	return m_position;
}

void Entity::SetPosition(const Vector2 newPos) {
	m_position = newPos;
}

bool Entity::CheckCollision(Entity* other) {
	if (GetGlobalBounds().Overlapping(other->GetGlobalBounds())) {
		m_hasCollided = true;
		return true;
	}
	return false;
}

BoundsRectangle Entity::GetGlobalBounds() const {
	return {
		m_position,
		{ m_position.x + static_cast<float>(m_size.x), m_position.y + static_cast<float>(m_size.y)}
	};
}