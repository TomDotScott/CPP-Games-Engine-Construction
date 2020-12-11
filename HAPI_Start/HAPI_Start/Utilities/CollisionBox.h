#pragma once
#include "Vector.h"

class CollisionBox
{
public:
	CollisionBox() = default;
	CollisionBox(Vector2 topLeft, Vector2 bottomRight);
	void ClipTo(const CollisionBox& other);
	bool Overlapping(const CollisionBox& other) const;
	bool IsCompletelyInside(const CollisionBox& other) const;
	bool IsCompletelyOutside(const CollisionBox& other) const;
	CollisionBox Translate(Vector2 dxy);
	Vector2 GetSize() const;

	Vector2 TOP_LEFT;
	Vector2 BOTTOM_RIGHT;
};
