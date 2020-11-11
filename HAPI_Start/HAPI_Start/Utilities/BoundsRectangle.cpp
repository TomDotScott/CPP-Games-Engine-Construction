#include "BoundsRectangle.h"
#include <utility>

BoundsRectangle::BoundsRectangle(const Vector2 topLeft,	const Vector2 bottomRight) :
	TOP_LEFT(topLeft),
	BOTTOM_RIGHT(bottomRight) {
}

void BoundsRectangle::ClipTo(const BoundsRectangle& other) {
	// Work out the bounds to clip to
	const auto x1 = std::max(TOP_LEFT.x, other.TOP_LEFT.x);
	const auto y1 = std::max(TOP_LEFT.y, other.TOP_LEFT.y);
	const auto x2 = std::min(BOTTOM_RIGHT.x, other.BOTTOM_RIGHT.x);
	const auto y2 = std::min(BOTTOM_RIGHT.y, other.BOTTOM_RIGHT.y);
	// Make the clipped bounds the new bounds
	TOP_LEFT = { x1, y1 };
	BOTTOM_RIGHT = { x2, y2 };
}

bool BoundsRectangle::Overlapping(const BoundsRectangle& other) const {
	return TOP_LEFT.x < other.BOTTOM_RIGHT.x&&
		other.TOP_LEFT.x < BOTTOM_RIGHT.x&&
		TOP_LEFT.y < other.BOTTOM_RIGHT.y&&
		other.TOP_LEFT.y < BOTTOM_RIGHT.y;
}

bool BoundsRectangle::IsCompletelyInside(const BoundsRectangle& other) const {
	return (TOP_LEFT.x > other.TOP_LEFT.x && TOP_LEFT.y > other.TOP_LEFT.y) &&
		(BOTTOM_RIGHT.x < other.BOTTOM_RIGHT.x&& BOTTOM_RIGHT.y < other.BOTTOM_RIGHT.y);
}

bool BoundsRectangle::IsCompletelyOutside(const BoundsRectangle& other) const {
	return (TOP_LEFT.x < other.TOP_LEFT.x && TOP_LEFT.y < other.TOP_LEFT.y) &&
		(BOTTOM_RIGHT.x > other.BOTTOM_RIGHT.x&& BOTTOM_RIGHT.y > other.BOTTOM_RIGHT.y);
}

void BoundsRectangle::Translate(const float dx, const float dy) {
	TOP_LEFT = { TOP_LEFT.x + dx, TOP_LEFT.y + dy };
	BOTTOM_RIGHT = { BOTTOM_RIGHT.x + dx, BOTTOM_RIGHT.y + dy };
}

Vector2 BoundsRectangle::GetSize() const {
	return{BOTTOM_RIGHT.x - TOP_LEFT.x, BOTTOM_RIGHT.y - TOP_LEFT.y};
}