#pragma once
#include "HAPI_Types.h"
class Vector2;

namespace renderer
{
	/**
	 * \brief Renders a texture to the screen
	 * \param screen A pointer to the screen data
	 * \param textureData The texture data to be rendered
	 * \param textureSize The size of the texture to be drawn
	 * \param texturePosition The position of the texture in screen-space
	 * \param flipped True if the texture should be flipped along the vertical
	 * axis
	 */
	void render_texture(HAPISPACE::BYTE* screen, HAPISPACE::BYTE* textureData, const Vector2& textureSize, Vector2 texturePosition, bool flipped);
	
	/**
	 * \brief Renders a sprite to the screen
	 * \param screen A pointer to the screen data
	 * \param spriteSheetData The texture data of the spritesheet to be rendered
	 * \param spriteSheetIndex The index in the spritesheet of the desired sprite
	 * \param spritePosition The position in screen-space of the sprite
	 * \param flipped True if the sprite should be flipped along the vertical axis
	 * \param alpha A value between 0 and 255 that overloads the alpha channel of the
	 * sprite data to give it transparency
	 */
	void render_sprite(HAPISPACE::BYTE* screen, HAPISPACE::BYTE* spriteSheetData, int spriteSheetIndex, Vector2 spritePosition, bool flipped, short alpha);
}
