#pragma once
#include "../Utilities/BoundsRectangle.h"
#include "../Utilities/Vector.h"

enum class EBrickType {
	eRed, eGreen
};

class Brick {
public:
	explicit Brick(Vector2 position, EBrickType type);
	Vector2 GetPosition() const;
	EBrickType GetType() const;
	BoundsRectangle GetGlobalBounds() const;
private:
	bool m_isActive;
	Vector2 m_position;
	EBrickType m_type;
};
