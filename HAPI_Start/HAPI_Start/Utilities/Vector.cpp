#include "Vector.h"
#include <cmath>

Vector2::Vector2(const float _x, const float _y) :
	x(_x),
	y(_y) {
}

void Vector2::Limit(Vector2 _max) {
	if (x > _max.x) {
		x = _max.x;
	}
	if (y > _max.y) {
		y = _max.y;
	}
}

float Vector2::Magnitude() const {
	return sqrt(std::pow(x, 2.f) + std::pow(y, 2.f));
}

void Vector2::Normalised() {
	x /= Magnitude();
	y /= Magnitude();

}

// Static Variables
Vector2 Vector2::UP = { 0.f, 1.f };
Vector2 Vector2::DOWN = { 0.f, -1.f };
Vector2 Vector2::LEFT = { -1.f, 0.f };
Vector2 Vector2::RIGHT = { 1.f, 0.f };
Vector2 Vector2::ZERO = { 0.f, 0.f };

// Operators
Vector2 Vector2::operator +(const Vector2& rhs) const {
	return{ this->x + rhs.x, this->y + rhs.y };
}

Vector2 Vector2::operator*(const float rhs) const {
	return{ this->x * rhs, this->y * rhs };
}

Vector2 Vector2::operator -(const Vector2& rhs) const {
	return{ rhs.x - this->x, rhs.y - this->y };
}

bool Vector2::operator==(Vector2& rhs) const
{
	return{ x == rhs.x && y == rhs.y };
}

bool Vector2::operator!=(Vector2& rhs) const
{
	return{ x != rhs.x || y != rhs.y };
}
