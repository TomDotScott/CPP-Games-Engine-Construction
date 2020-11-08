#pragma once
#include <cstdlib>

namespace constants {
	constexpr int k_screenWidth = 512;
	constexpr int k_screenHeight = 640;
	constexpr int k_borderWidth = 20;

	constexpr int k_spriteSheetCellWith = 64;
	
	constexpr float k_leftThumbDeadzone = 7849.f;
	constexpr float k_rightThumbDeadzone = 8689.f;

	inline int rand_range(const int min, const int max) {
		return min + (rand() % (max - min + 1));
	}
}
