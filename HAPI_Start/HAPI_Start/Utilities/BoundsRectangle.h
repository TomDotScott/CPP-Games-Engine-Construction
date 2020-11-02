#pragma once
#include "Vector.h"

class BoundsRectangle {
public:
	BoundsRectangle(const Vector2 topLeft, const Vector2 bottomRight);
	void ClipToBound(const BoundsRectangle& other);
	bool Overlapping(const BoundsRectangle& other) const;
	bool CompletelyInside(const BoundsRectangle& other) const;
	bool CompletelyOutside(const BoundsRectangle& other) const;
	void Translate(float dx, float dy);
	Vector2 GetSize() const;
	Vector2 GetTopLeft() const;
private:
	Vector2 m_topLeft;
	Vector2 m_bottomRight;
};
