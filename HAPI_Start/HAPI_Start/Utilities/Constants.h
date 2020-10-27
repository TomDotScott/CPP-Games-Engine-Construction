#pragma once
#include <cstdlib>

namespace constants {
	constexpr int k_screenWidth = 1024;
	constexpr int k_screenHeight = 768;
	constexpr int k_borderWidth = 28;
	constexpr float k_leftThumbDeadzone = 7849.f;
	constexpr float k_rightThumbDeadzone = 8689.f;

	inline int rand_range(const int min, const int max) {
		return min + (rand() % (max - min + 1));
	}
}
