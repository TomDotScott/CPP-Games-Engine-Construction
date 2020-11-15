#pragma once
#include <cstdlib>

namespace constants {
	constexpr int k_screenWidth = 1280;
	constexpr int k_screenHeight = 960;
	
	constexpr int k_spriteSheetCellWith = 64;

	constexpr int k_chunkWidth = k_screenWidth / k_spriteSheetCellWith;
	constexpr int k_chunkHeight = k_screenHeight / k_spriteSheetCellWith;
	
	constexpr float k_leftThumbDeadzone = 7849.f;
	constexpr float k_rightThumbDeadzone = 8689.f;

	constexpr float k_gravity = 9.f;
	
	inline int rand_range(const int min, const int max) {
		return min + (rand() % (max - min + 1));
	}
}
