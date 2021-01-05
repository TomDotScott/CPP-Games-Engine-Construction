#pragma once
#include <iostream>

/**
 * \brief A mathematical vector class of 2 dimensions
 */
class Vector2
{
public:
	/**
	 * \brief Constructs a Vector2 object
	 * \param x the initial x value
	 * \param y the initial y value
	 */
	Vector2(float x = 0.f, float y = 0.f);

	/**
	 * \brief Limits the x and y components to a maximum
	 * \param max the x and y maximum for the components
	 */
	void Limit(Vector2 max);

	/**
	 * \brief Calculates the magnitude of the vector
	 * \return The magnitude of the vector
	 */
	float Magnitude() const;

	/**
	 * \brief Normalises the vector so that the magnitude is between 0 and 1
	 */
	void Normalised();

	friend std::ostream& operator<<(std::ostream& os, const Vector2& dt);

	Vector2 operator+(const Vector2& rhs) const;
	Vector2 operator*(float rhs) const;
	Vector2 operator-(const Vector2& rhs) const;
	bool    operator==(Vector2& rhs) const;
	bool    operator!=(Vector2& rhs) const;

	static const Vector2 UP;
	static const Vector2 DOWN;
	static const Vector2 LEFT;
	static const Vector2 RIGHT;
	static const Vector2 ZERO;
	static const Vector2 CENTRE;

	float x;
	float y;
};
