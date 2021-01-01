#pragma once
#include <iostream>

class Vector2
{
public:
	Vector2(float x = 0.f, float y = 0.f);

	void Limit(Vector2 max);

	float Magnitude() const;

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
