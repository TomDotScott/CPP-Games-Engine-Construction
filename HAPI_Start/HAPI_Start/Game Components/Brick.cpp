#include "Brick.h"
#include "../Graphics/Graphics.h"

Brick::Brick(const Vector2 position) : m_position(position), m_isActive(true)
{
}

void Brick::Render() {
	if(m_isActive) {
		Graphics::GetInstance().DrawSprite("Brick", m_position);
	}
}
