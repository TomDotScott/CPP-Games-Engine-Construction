#pragma once

class Vector2 {
public:
	Vector2(const float _x = 0.f, const float _y = 0.f);

	void Limit(Vector2 _max);

	float Magnitude() const;

	void Normalised();

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

	float x, y;
};