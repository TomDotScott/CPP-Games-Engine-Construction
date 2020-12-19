#pragma once
#include "HAPI_Types.h"
class Vector2;

namespace renderer
{
	void render_texture(HAPISPACE::BYTE* screen, HAPISPACE::BYTE* textureData, Vector2 textureSize, Vector2 texturePosition, bool flipped);
	void render_sprite(HAPISPACE::BYTE* screen, HAPISPACE::BYTE* spriteSheetData, int spriteSheetIndex, Vector2 spritePosition, bool flipped, short alpha);
}

