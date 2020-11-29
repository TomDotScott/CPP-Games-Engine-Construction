#pragma once
#include <iostream>

class Vector2 {
public:
	Vector2(const float x = 0.f, const float y = 0.f);

	void Limit(Vector2 max);

	float Magnitude() const;

	void Normalised();
	
	friend std::ostream& operator<<(std::ostream& os, const Vector2& dt);
	
	Vector2 operator+(const Vector2& rhs) const;
	Vector2 operator*(const float rhs) const;
	Vector2 operator-(const Vector2& rhs) const;
	bool operator==(Vector2& rhs) const;
	bool operator!=(Vector2& rhs) const;
	static Vector2 UP;
	static Vector2 DOWN;
	static Vector2 LEFT;
	static Vector2 RIGHT;
	static Vector2 ZERO;
	static Vector2 CENTRE;

	float x, y;
};