#include "BoundsRectangle.h"

BoundsRectangle::BoundsRectangle(const Vector2 topLeft,
	const Vector2 topRight,
	const Vector2 bottomLeft,
	const Vector2 bottomRight) :
	m_topLeft(topLeft),
	m_topRight(topRight),
	m_bottomLeft(bottomLeft),
	m_bottomRight(bottomRight) {
}

bool BoundsRectangle::Overlapping(const BoundsRectangle& other) const {
	return m_topLeft.x < other.m_bottomRight.x&&
		other.m_topLeft.x < m_bottomRight.x&&
		m_topLeft.y < other.m_bottomRight.y&&
		other.m_topLeft.y < m_bottomRight.y;
}

bool BoundsRectangle::CompletelyInside(const BoundsRectangle& other) const {
	return other.m_topLeft.x > m_topLeft.x &&
		other.m_topRight.x < m_topRight.x&&
		other.m_bottomLeft.x > m_bottomLeft.x &&
		other.m_bottomRight.x < m_bottomRight.x&&


		other.m_topLeft.y > m_topLeft.y &&
		other.m_topRight.y > m_topRight.y &&
		other.m_bottomLeft.y < m_bottomLeft.y&&
		other.m_bottomRight.y < m_bottomRight.y;
};
