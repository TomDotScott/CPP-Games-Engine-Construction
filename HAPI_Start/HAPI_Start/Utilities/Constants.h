#pragma once
#include <random>

namespace constants
{
	constexpr int k_screenWidth = 1280;
	constexpr int k_screenHeight = 960;

	constexpr int k_spriteSheetCellSize = 64;

	constexpr int k_backgroundTileWidth = 960;

	constexpr int k_maxTilesHorizontal = k_screenWidth / k_spriteSheetCellSize;
	constexpr int k_maxTilesVertical = k_screenHeight / k_spriteSheetCellSize;

	// Controller Info
	constexpr float k_leftThumbDeadzone = 7849.f;
	constexpr float k_rightThumbDeadzone = 8689.f;

	constexpr float k_gravity = 25.f;

	constexpr float k_fireBallCoolDownTimer = 0.5f;
	
	inline int rand_range(const int min, const int max)
	{
		return min + (rand() % (max - min + 1));
	}
}
