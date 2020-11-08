#include "Brick.h"

Brick::Brick(const Vector2 position, const EBrickType type) : m_isActive(true), m_position(position), m_type(type)
{
}

Vector2 Brick::GetPosition() const {
	return m_position;
}

EBrickType Brick::GetType() const {
	return m_type;
}

BoundsRectangle Brick::GetGlobalBounds() const {
	return {
		m_position,
		{ m_position.x + 64.f, m_position.y + 64.f }
	};
}
