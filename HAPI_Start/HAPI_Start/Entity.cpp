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

void Entity::Update() {
	return;
}

void Entity::Render(HAPISPACE::BYTE* _screen) {
	m_texture->Render(_screen);
}
