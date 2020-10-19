#pragma once
#include <cstdlib>

namespace constants
{
	constexpr int k_screenWidth = 1024;
	constexpr int k_screenHeight = 768;

	inline int rand_range(const int _min, const int _max) {
		return _min + (rand() % (_max - _min + 1));
	}
}
