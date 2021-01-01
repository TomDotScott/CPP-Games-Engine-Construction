#include "Vector.h"
#include <cmath>
#include "Constants.h"

Vector2::Vector2(const float x, const float y) :
	x(x),
	y(y) {}

void Vector2::Limit(const Vector2 max)
{
	if (x > max.x)
	{
		x = max.x;
	}
	if (y > max.y)
	{
		y = max.y;
	}
}

float Vector2::Magnitude() const
{
	return sqrt(std::pow(x, 2.f) + std::pow(y, 2.f));
}

void Vector2::Normalised()
{
	x /= Magnitude();
	y /= Magnitude();
}

// Static Variables
Vector2 const Vector2::UP{0.f, -1.f};
Vector2 const Vector2::DOWN{0.f, 1.f};
Vector2 const Vector2::LEFT{-1.f, 0.f};
Vector2 const Vector2::RIGHT{1.f, 0.f};
Vector2 const Vector2::ZERO{0.f, 0.f};
Vector2 const Vector2::CENTRE{
	static_cast<float>(constants::k_screenWidth) / 2.f, static_cast<float>(constants::k_screenHeight) / 2.f
};

// Operators
Vector2 Vector2::operator +(const Vector2& rhs) const
{
	return {this->x + rhs.x, this->y + rhs.y};
}

Vector2 Vector2::operator*(const float rhs) const
{
	return {this->x * rhs, this->y * rhs};
}

Vector2 Vector2::operator -(const Vector2& rhs) const
{
	return {rhs.x - this->x, rhs.y - this->y};
}

bool Vector2::operator==(Vector2& rhs) const
{
	return {x == rhs.x && y == rhs.y};
}

bool Vector2::operator!=(Vector2& rhs) const
{
	return {x != rhs.x || y != rhs.y};
}

std::ostream& operator<<(std::ostream& os, const Vector2& dt)
{
	return os << "X: " << dt.x << " Y: " << dt.y;
}
