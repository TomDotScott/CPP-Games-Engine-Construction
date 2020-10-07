#pragma once
struct Vector3
{
	Vector3(const int _x = 0, const int _y = 0, const int _z = 0) :
		x(_x),
		y(_y),
		z(_z) {
	};
	int x, y, z;
};
