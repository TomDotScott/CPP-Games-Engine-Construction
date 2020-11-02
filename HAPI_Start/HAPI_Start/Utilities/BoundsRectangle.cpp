#include "BoundsRectangle.h"
#include <utility>

BoundsRectangle::BoundsRectangle(const Vector2 topLeft,	const Vector2 bottomRight) :
	m_topLeft(topLeft),
	m_bottomRight(bottomRight) {
}

void BoundsRectangle::ClipToBound(const BoundsRectangle& other) {
	// Work out the bounds to clip to
	const auto x1 = std::max(m_topLeft.x, other.m_topLeft.x);
	const auto y1 = std::max(m_topLeft.y, other.m_topLeft.y);
	const auto x2 = std::min(m_bottomRight.x, other.m_bottomRight.x);
	const auto y2 = std::min(m_bottomRight.y, other.m_bottomRight.y);
	// Make the clipped bounds the new bounds
	m_topLeft = { x1, y1 };
	m_bottomRight = { x2, y2 };
}

bool BoundsRectangle::Overlapping(const BoundsRectangle& other) const {
	return m_topLeft.x < other.m_bottomRight.x&&
		other.m_topLeft.x < m_bottomRight.x&&
		m_topLeft.y < other.m_bottomRight.y&&
		other.m_topLeft.y < m_bottomRight.y;
}

bool BoundsRectangle::CompletelyInside(const BoundsRectangle& other) const {
	return (m_topLeft.x > other.m_topLeft.x && m_topLeft.y > other.m_topLeft.y) &&
		(m_bottomRight.x < other.m_bottomRight.x&& m_bottomRight.y < other.m_bottomRight.y);
}

bool BoundsRectangle::CompletelyOutside(const BoundsRectangle& other) const {
	return (m_topLeft.x < other.m_topLeft.x && m_topLeft.y < other.m_topLeft.y) &&
		(m_bottomRight.x > other.m_bottomRight.x&& m_bottomRight.y > other.m_bottomRight.y);
}

void BoundsRectangle::Translate(const float dx, const float dy) {
	m_topLeft = { m_topLeft.x + dx, m_topLeft.y + dy };
	m_bottomRight = { m_bottomRight.x + dx, m_bottomRight.y + dy };
}

Vector2 BoundsRectangle::GetSize() const {
	return{m_bottomRight.x - m_topLeft.x, m_bottomRight.y - m_topLeft.y};
}

Vector2 BoundsRectangle::GetTopLeft() const {
	return m_topLeft;
};
