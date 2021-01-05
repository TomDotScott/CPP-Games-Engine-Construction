#pragma once
#include "Vector.h"

/**
 * \brief CollisionBox is a basic rectangle class, with vector2 positions
 * for the top left and bottom right as well as helper methods
 * for checking for overlaps and unions
 */
class CollisionBox
{
public:
	CollisionBox() = default;
	CollisionBox(const Vector2& topLeft, const Vector2& bottomRight);

	/**
	 * \brief Clips the box to be completely within the
	 * other rectangle
	 * \param other The rectangle to clip to
	 */
	void         ClipTo(const CollisionBox& other);
	/**
	 * \brief Checks whether rectangles overlap each other
	 * \param other The rectangle to check against
	 * \return True if the rectangles overlap each other
	 */
	bool         Overlapping(const CollisionBox& other) const;
	/**
	 * \brief Checks whether the rectangle is completely inside another
	 * \param other The rectangle to check against
	 * \return True if the rectangle is completely inside the other
	 */
	bool         IsCompletelyInside(const CollisionBox& other) const;
	/**
	 * \brief Checks whether the rectangle is completely outside of another
	 * \param other The rectangle to check against
	 * \return True if the rectangle is completely outside of the other
	 */
	bool         IsCompletelyOutside(const CollisionBox& other) const;
	/**
	 * \brief Translates a rectangle by a given amount
	 * \param dxy The difference in x and y of the translation
	 * \return The translated collision box
	 */
	CollisionBox Translate(const Vector2& dxy);

	/**
	 * \brief Calculates the size of the collision box
	 * \return A Vector2 {WIDTH, HEIGHT}
	 */
	Vector2 GetSize() const;

	Vector2 TOP_LEFT;
	Vector2 BOTTOM_RIGHT;
};
