#include "Brick.h"

Brick::Brick(const Vector2 position, const EBrickType type) :
	Entity(type == EBrickType::eRed ? "RedBrick" : "GreenBrick",
		Vector2(constants::k_spriteSheetCellWith, constants::k_spriteSheetCellWith),
		position
	),
	m_isActive(true),
	m_position(position), m_type(type) {
}

void Brick::Update(float deltaTime) {
	
}

EBrickType Brick::GetType() const {
	return m_type;
}

bool Brick::GetIsActive() const {
	return m_isActive;
}

void Brick::CheckCollision(const BoundsRectangle other, int& gameScore) {
	if(Entity::CheckCollisions(other)) {
		if(m_type == EBrickType::eGreen) {
			m_type = EBrickType::eRed;
			gameScore += 32;
		}else {
			m_isActive = false;
			gameScore += 64;
		}
	}
}
