#pragma once
struct Vector3
{
	Vector3(int _x = 0, int _y = 0, int _z = 0) :
		x(_x),
		y(_y),
		z(_z) {
	};
	int x, y, z;
};
