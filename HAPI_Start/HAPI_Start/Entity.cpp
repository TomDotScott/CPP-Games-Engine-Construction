#include "Entity.h"

Entity::Entity(const std::string& _fileName, const Vector2 _position, const Vector2 _velocity) :
	m_texture(nullptr),
	m_position(_position),
	m_velocity(_velocity) {

	m_texture = new Texture(_fileName, m_position);
}

Entity::~Entity() {
	delete m_texture;
}

void Entity::Render(HAPISPACE::BYTE* _screen) {
	m_texture->Render(_screen);
}

void Entity::SetPosition(const Vector2 _newPos) {
	m_position = _newPos;
	m_texture->SetPosition(_newPos);
}

bool Entity::CheckCollision(Entity* _other) const {
	const auto bounds = this->m_texture->GetGlobalBounds();
	const auto otherBounds = _other->m_texture->GetGlobalBounds();

	return bounds.first.x < otherBounds.second.x&&
		otherBounds.first.x < bounds.second.x&&
		bounds.first.y < otherBounds.second.y&&
		otherBounds.first.y < bounds.second.y;
}
