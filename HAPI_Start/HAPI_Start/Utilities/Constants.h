#pragma once
#include <random>

namespace constants
{
	constexpr int   k_screenWidth           = 1280;
	constexpr int   k_screenHeight          = 960;
	
	constexpr int   k_spriteSheetCellSize   = 64;
	
	constexpr int   k_backgroundTileWidth   = 960;
	constexpr int   k_maxTilesHorizontal    = k_screenWidth / k_spriteSheetCellSize;
	constexpr int   k_maxTilesVertical      = k_screenHeight / k_spriteSheetCellSize;
	
	constexpr float k_leftThumbDeadzone     = 7849.f;
	constexpr float k_rightThumbDeadzone    = 8689.f;
	
	constexpr float k_gravity               = 25.f;
	constexpr float k_fireBallCoolDownTimer = 0.5f;
	
	constexpr int   k_bufferSamples         = 8192;
	constexpr int   k_numBuffers            = 4;
	
	constexpr float k_bossMinX              = 23552.f;
	constexpr float k_bossMaxX              = 24128.f;
	
	constexpr float k_levelMinX             = 0.f;
	constexpr float k_levelOneMaxX          = 13376.f;
	constexpr float k_levelTwoMaxX          = 12736.f;
	constexpr float k_levelThreeMaxX        = 26048.f;

	inline int rand_range(const int min, const int max)
	{
		return min + (rand() % (max - min + 1));
	}
}
