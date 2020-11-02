#pragma once
#include "Vector.h"

class BoundsRectangle {
public:
	BoundsRectangle(const Vector2 topLeft, const Vector2 topRight, const Vector2 bottomLeft, const Vector2 bottomRight);
	bool Overlapping(const BoundsRectangle& other) const;
	bool CompletelyInside(const BoundsRectangle& other) const;
	
private:
	Vector2 m_topLeft;
	Vector2 m_topRight;
	Vector2 m_bottomLeft;
	Vector2 m_bottomRight;
};
