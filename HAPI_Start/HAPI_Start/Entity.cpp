#include "Entity.h"

Entity::Entity(const std::string& _fileName, const Vector2 _position, const Vector2 _velocity, const Vector2 _acceleration) :
	m_texture(),
	m_position(_position),
	m_velocity(_velocity),
	m_acceleration(_acceleration),
	m_physicsClock(){
	m_texture = new Texture(_fileName, m_position);
}

Entity::~Entity() {
	delete m_texture;
}

void Entity::Render(HAPISPACE::BYTE* _screen) const{
	m_texture->Render(_screen);
}

Vector2 Entity::GetPosition() const
{
	return m_position;
}

void Entity::SetPosition(const Vector2 _newPos) {
	m_position = _newPos;
	m_texture->SetPosition(_newPos);
}

bool Entity::CheckCollision(Entity* _other) const {
	const auto bounds = this->m_texture->GetGlobalBounds();
	const auto otherBounds = _other->m_texture->GetGlobalBounds();

	return bounds.m_topLeft.x < otherBounds.m_bottomRight.x &&
		otherBounds.m_topLeft.x < bounds.m_bottomRight.x &&
		bounds.m_topLeft.y < otherBounds.m_bottomRight.y&&
		otherBounds.m_topLeft.y < bounds.m_bottomRight.y;
}
