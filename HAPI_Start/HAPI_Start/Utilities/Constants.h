#pragma once
#include <cstdlib>

namespace constants {
	constexpr int k_screenWidth = 1920;
	constexpr int k_screenHeight = 1080;
	constexpr int k_borderWidth = 20;

	constexpr int k_spriteSheetCellWith = 128;
	
	constexpr float k_leftThumbDeadzone = 7849.f;
	constexpr float k_rightThumbDeadzone = 8689.f;

	constexpr float k_gravity = 9.f;
	
	inline int rand_range(const int min, const int max) {
		return min + (rand() % (max - min + 1));
	}
}
