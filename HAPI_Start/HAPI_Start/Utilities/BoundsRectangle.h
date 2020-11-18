#pragma once
#include "Vector.h"

class BoundsRectangle {
public:
	BoundsRectangle(Vector2 topLeft, Vector2 bottomRight);
	void ClipTo(const BoundsRectangle& other);
	bool Overlapping(const BoundsRectangle& other) const;
	bool IsCompletelyInside(const BoundsRectangle& other) const;
	bool IsCompletelyOutside(const BoundsRectangle& other) const;
	void Translate(float dx, float dy);
	Vector2 GetSize() const;

	Vector2 TOP_LEFT;
	Vector2 BOTTOM_RIGHT;	
};
