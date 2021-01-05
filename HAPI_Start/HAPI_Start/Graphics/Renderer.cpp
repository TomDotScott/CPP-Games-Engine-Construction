#include "Renderer.h"
#include "../Utilities/CollisionBox.h"
#include "../Utilities/Constants.h"
#include "../Utilities/Vector.h"

/**
 * \brief The renderer namespace details the rendering of both
 * textures and sprites to the screen
 */
namespace renderer
{
	/**
	 * \brief The texture namespace details the functions that can be used
	 * render textures to the screen
	 */
	namespace texture
	{
		/**
		 * \brief Flips the texture along the vertical axis
		 * \param screen A pointer to the screen data
		 * \param textureData The data of the texture to be flipped
		 * \param textureSize The size of the texture to be flipped
		 */
		void flip(HAPISPACE::BYTE* screen, HAPISPACE::BYTE* textureData, const Vector2& textureSize)
		{
			const int width  = static_cast<int>(textureSize.x);
			const int height = static_cast<int>(textureSize.y);

			const int imgWidthHalf = width / 2;
			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < imgWidthHalf; x++)
				{
					const int  xy1       = (x + y * width) * 4;
					const int  xy2       = ((width - 1 - x) + y * width) * 4;
					const auto red       = textureData[xy1];
					const auto green     = textureData[xy1 + 1];
					const auto blue      = textureData[xy1 + 2];
					const auto alpha     = textureData[xy1 + 3];
					textureData[xy1]     = textureData[xy2];
					textureData[xy1 + 1] = textureData[xy2 + 1];
					textureData[xy1 + 2] = textureData[xy2 + 2];
					textureData[xy1 + 3] = textureData[xy2 + 3];
					textureData[xy2]     = red;
					textureData[xy2 + 1] = green;
					textureData[xy2 + 2] = blue;
					textureData[xy2 + 3] = alpha;
				}
			}
		}

		/**
		 * \brief Performs a fast blit to the screen accounting for potential alpha channels
		 * in the texture
		 * \param screen A pointer to the screen data
		 * \param textureData The data of the texture to be blitted
		 * \param textureSize The size of the texture to be blitted
		 * \param texturePosition The position in screen-space of the
		 * texture to be blitted
		 */
		void alpha_blit(HAPISPACE::BYTE* screen, HAPISPACE::BYTE* textureData, const Vector2& textureSize,
		                const Vector2&   texturePosition)
		{
			HAPISPACE::BYTE* screenStart{
				screen + (static_cast<uint64_t>(texturePosition.y) * constants::k_screenWidth + static_cast<uint64_t>(texturePosition.x)) * 4
			};

			HAPISPACE::BYTE* textureStart{textureData};

			const int increment{constants::k_screenWidth * 4 - static_cast<int>(textureSize.x) * 4};

			for (int y = 0; y < static_cast<int>(textureSize.y); y++)
			{
				for (int x = 0; x < static_cast<int>(textureSize.x); x++)
				{
					const HAPISPACE::BYTE a{textureStart[3]};
					// Only draw pixels if needed
					if (a > 0)
					{
						// Fast blit if no alpha
						if (a == 255)
						{
							memcpy(screenStart, textureStart, 4);
						}
						else
						{
							// Blend with background
							screenStart[0] = screenStart[0] + ((a * (textureStart[2] - screenStart[0])) >> 8);
							screenStart[1] = screenStart[1] + ((a * (textureStart[1] - screenStart[1])) >> 8);
							screenStart[2] = screenStart[2] + ((a * (textureStart[0] - screenStart[2])) >> 8);
						}
					}
					screenStart += 4;
					textureStart += 4;
				}
				screenStart += increment;
			}
		}

		/**
		 * \brief Performs a clip-blit (where the texture is partially offscreen)
		 * to the texture data
		 * \param screen A pointer to the screen data
		 * \param textureData The data of the texture
		 * \param textureSize The size of the texture
		 * \param texturePosition The position of the texture in screen-space
		 */
		void clip_blit(HAPISPACE::BYTE* screen, HAPISPACE::BYTE* textureData, const Vector2& textureSize,
		               Vector2          texturePosition)
		{
			// BoundsRectangle takes in coordinates for the top left and bottom right
			// My Vector2 class defaults to zeros
			const Vector2      temp = texturePosition;
			const CollisionBox textureBounds({}, textureSize);

			const CollisionBox screenBounds({}, {constants::k_screenWidth, constants::k_screenHeight});

			// Create a copy to clip with 
			CollisionBox clippedRect(textureBounds);

			//Translate to screen space
			clippedRect.Translate(temp);

			// If the object is onscreen...
			if (!clippedRect.IsCompletelyOutside(screenBounds))
			{
				// If the object is completely onscreen then alpha-blit...
				if (clippedRect.IsCompletelyInside(screenBounds))
				{
					alpha_blit(screen, textureData, textureSize, texturePosition);
				}
				else
				{
					// we must be offscreen...
					//Clip against screen
					clippedRect.ClipTo(screenBounds);

					clippedRect.Translate(temp * -1);

					//Clamping to negative
					texturePosition.x = std::max(0.f, temp.x);
					texturePosition.y = std::max(0.f, temp.y);

					// Calculate offsets and starting points for screen and texture....
					const int screenOffset{
						(static_cast<int>(texturePosition.x) + static_cast<int>(texturePosition.y) * static_cast<int>(
							 screenBounds.GetSize().x)) * 4
					};

					HAPISPACE::BYTE* screenPtr = screen + screenOffset;

					const int textureOffset{
						(static_cast<int>(clippedRect.TOP_LEFT.x) + static_cast<int>(clippedRect.TOP_LEFT.y) *
						 static_cast<int>(textureBounds.GetSize().x)) * 4
					};

					HAPISPACE::BYTE* texPtr = textureData + textureOffset;

					// Calculate the increment up here rather than in the loop since it doesn't change...
					const int screenInc{
						(static_cast<int>(screenBounds.GetSize().x) - static_cast<int>(clippedRect.GetSize().x)) * 4
					};

					const int texInc{
						(static_cast<int>(textureBounds.GetSize().x) - static_cast<int>(clippedRect.GetSize().x)) * 4
					};

					// Start blitting...
					for (int y = 0; y < static_cast<int>(clippedRect.GetSize().y); y++)
					{
						for (int x = 0; x < static_cast<int>(clippedRect.GetSize().x); x++)
						{
							const HAPISPACE::BYTE alpha = texPtr[3];
							// Fully opaque
							if (alpha == 255)
							{
								memcpy(screenPtr, texPtr, 4);
							}
							else if (alpha > 0)
							{
								// Has alpha channel
								const HAPISPACE::BYTE red   = texPtr[0];
								const HAPISPACE::BYTE green = texPtr[1];
								const HAPISPACE::BYTE blue  = texPtr[2];

								screenPtr[0] = screenPtr[0] + ((alpha * (red - screenPtr[0])) >> 8);
								screenPtr[1] = screenPtr[1] + ((alpha * (green - screenPtr[1])) >> 8);
								screenPtr[2] = screenPtr[2] + ((alpha * (blue - screenPtr[2])) >> 8);
							}
							texPtr += 4;
							screenPtr += 4;
						}

						screenPtr += screenInc;
						texPtr += texInc;
					}
				}
			}
		}
	}

	/**
	 * \brief The sprite_sheet namespace deals with the rendering
	 * of spritesheet data 
	 */
	namespace sprite_sheet
	{
		/**
		 * \brief Flips the sprite at a specific index along the vertical axis
		 * \param spriteSheetData The spritesheet data
		 * \param spriteSheetLocation The index of the sprite to be flipped
		 */
		void flip(HAPISPACE::BYTE* spriteSheetData, const int spriteSheetLocation)
		{
			HAPISPACE::BYTE* spriteData{
				spriteSheetData + static_cast<uint64_t>(
					constants::k_spriteSheetCellSize * constants::k_spriteSheetCellSize * 4) * spriteSheetLocation
			};

			const int imgWidthHalf = constants::k_spriteSheetCellSize / 2;

			for (int y = 0; y < constants::k_spriteSheetCellSize; y++)
			{
				for (int x = 0; x < imgWidthHalf; x++)
				{
					const int xy1 = (x + y * constants::k_spriteSheetCellSize) * 4;
					const int xy2 = ((constants::k_spriteSheetCellSize - 1 - x) + y * constants::k_spriteSheetCellSize)
					                * 4;

					const auto red   = spriteData[xy1];
					const auto green = spriteData[xy1 + 1];
					const auto blue  = spriteData[xy1 + 2];
					const auto alpha = spriteData[xy1 + 3];

					spriteData[xy1]     = spriteData[xy2];
					spriteData[xy1 + 1] = spriteData[xy2 + 1];
					spriteData[xy1 + 2] = spriteData[xy2 + 2];
					spriteData[xy1 + 3] = spriteData[xy2 + 3];

					spriteData[xy2]     = red;
					spriteData[xy2 + 1] = green;
					spriteData[xy2 + 2] = blue;
					spriteData[xy2 + 3] = alpha;
				}
			}
		}
		
		/**
		 * \brief Performs a fast blit to the screen accounting for potential alpha channels
		 * in the sprite
		 * \param screen a pointer to the screen data 
		 * \param spriteData The data of the sprite
		 * \param position The position in screen-space of the sprite
		 * \param alpha The alpha value to overload
		 */
		void alpha_blit(HAPISPACE::BYTE* screen, HAPISPACE::BYTE* spriteData, const Vector2 position, const short alpha = 255)
		{
			HAPISPACE::BYTE* screenStart{
				screen + (static_cast<uint64_t>(position.x) + static_cast<uint64_t>(position.y) *
				          constants::k_screenWidth) * 4
			};

			const int screenInc{constants::k_screenWidth * 4 - constants::k_spriteSheetCellSize * 4};

			// const int spriteInc{ static_cast<int>(m_size.x) * 4 - constants::k_spriteSheetCellSize * 4 };

			for (int y = 0; y < constants::k_spriteSheetCellSize; y++)
			{
				for (int x = 0; x < constants::k_spriteSheetCellSize; x++)
				{
					HAPISPACE::BYTE a{spriteData[3]};
					if (alpha != 255)
					{
						const float normalised = static_cast<float>(alpha) / 255.f;
						a                      = static_cast<unsigned char>(static_cast<float>(a) * normalised);
					}

					// Only draw pixels if needed
					if (a > 0)
					{
						// Fast blit if no alpha
						if (a == 255)
						{
							memcpy(screenStart, spriteData, 4);
						}
						else
						{
							// Blend with background
							screenStart[0] = screenStart[0] + ((a * (spriteData[2] - screenStart[0])) >> 8);
							screenStart[1] = screenStart[1] + ((a * (spriteData[1] - screenStart[1])) >> 8);
							screenStart[2] = screenStart[2] + ((a * (spriteData[0] - screenStart[2])) >> 8);
						}
					}
					screenStart += 4;
					spriteData += 4;
				}
				screenStart += screenInc;
			}
		}

		/**
		 * \brief Performs a clip-blit (where the sprite is partially offscreen)
		 * to the sprite data
		 * \param screen A pointer to the screen data
		 * \param spriteData The spritesheet data to be clip-blitted
		 * \param spriteSheetIndex The index of the spritesheet of the desired
		 * sprite
		 * \param position The position in screen-space of the sprite
		 * \param alpha The overloaded alpha channel
		 */
		void clip_blit(HAPISPACE::BYTE* screen, HAPISPACE::BYTE* spriteData, int spriteSheetIndex, Vector2 position,
		               const short      alpha = 255)
		{
			// BoundsRectangle takes in coordinates for the top left and bottom right
			// My Vector2 class defaults to zeros
			const Vector2 temp = position;

			const CollisionBox spriteBounds({}, {
				                                static_cast<float>(constants::k_spriteSheetCellSize),
				                                static_cast<float>(constants::k_spriteSheetCellSize)
			                                });

			const CollisionBox screenBounds({}, {constants::k_screenWidth, constants::k_screenHeight});

			// Create a copy to clip with 
			CollisionBox clippedRect(spriteBounds);

			//Translate to screen space
			clippedRect.Translate(temp);

			// If the object is onscreen...
			if (!clippedRect.IsCompletelyOutside(screenBounds))
			{
				// If the object is completely onscreen then alpha-blit...
				if (clippedRect.IsCompletelyInside(screenBounds))
				{
					alpha_blit(screen, spriteData, position, alpha);
				}
				else
				{
					// we must be offscreen...
					//Clip against screen
					clippedRect.ClipTo(screenBounds);

					clippedRect.Translate(temp * -1);

					//Clamping to negative
					position.x = std::max(0.f, temp.x);
					position.y = std::max(0.f, temp.y);

					HAPISPACE::BYTE* screenStart{
						screen + (static_cast<uint64_t>(position.x) + static_cast<uint64_t>(position.y) *
						          constants::k_screenWidth) * 4
					};

					// Calculate the increment up here rather than in the loop since it doesn't change...
					const int screenInc{
						(static_cast<int>(screenBounds.GetSize().x) - static_cast<int>(clippedRect.GetSize().x)) * 4
					};

					const int spriteOffset{
						(static_cast<int>(clippedRect.TOP_LEFT.y) * constants::k_spriteSheetCellSize + static_cast<int>(
							 clippedRect.TOP_LEFT.x)) * 4
					};

					HAPISPACE::BYTE* spritePtr = spriteData + spriteOffset;

					const int spriteInc{
						((constants::k_spriteSheetCellSize - static_cast<int>(clippedRect.GetSize().x))) * 4
					};

					// Start blitting...
					for (int y = 0; y < static_cast<int>(clippedRect.GetSize().y); y++)
					{
						for (int x = 0; x < static_cast<int>(clippedRect.GetSize().x); x++)
						{
							HAPISPACE::BYTE a = spritePtr[3];
							if (alpha != 255)
							{
								const float normalised = static_cast<float>(alpha) / 255.f;
								a                      = static_cast<unsigned char>(static_cast<float>(a) * normalised);
							}
							// Fully opaque
							if (a == 255)
							{
								memcpy(screenStart, spritePtr, 4);
							}
							else if (a > 0)
							{
								// Has alpha channel
								const HAPISPACE::BYTE red   = spritePtr[0];
								const HAPISPACE::BYTE green = spritePtr[1];
								const HAPISPACE::BYTE blue  = spritePtr[2];

								screenStart[0] = screenStart[0] + ((a * (red - screenStart[0])) >> 8);
								screenStart[1] = screenStart[1] + ((a * (green - screenStart[1])) >> 8);
								screenStart[2] = screenStart[2] + ((a * (blue - screenStart[2])) >> 8);
							}
							spritePtr += 4;
							screenStart += 4;
						}
						screenStart += screenInc;
						spritePtr += spriteInc;
					}
				}
			}
		}
	}

	/**
	 * \brief Renders a texture to the screen using the appropriate blitting
	 * method
	 * \param screen A pointer to the screen data
	 * \param textureData The texture data to be rendered
	 * \param textureSize The size of the texture to be rendered
	 * \param texturePosition The position of the texture in screen-space of the
	 * texture
	 * \param flipped True if the texture should be flipped along the vertical
	 * axis
	 */
	void render_texture(HAPISPACE::BYTE* screen, HAPISPACE::BYTE*    textureData, const Vector2& textureSize,
	                    const Vector2    texturePosition, const bool flipped)
	{
		if (flipped)
		{
			texture::flip(screen, textureData, textureSize);
		}

		texture::clip_blit(screen, textureData, textureSize, texturePosition);

		if (flipped)
		{
			texture::flip(screen, textureData, textureSize);
		}
	}

	/**
	 * \brief Renders a sprite to the screen using the appropriate blitting
	 * method
	 * \param screen A pointer to the screen data
	 * \param spriteSheetData The spritesheet data
	 * \param spriteSheetIndex The index of the desired sprite in the spritesheet
	 * \param spritePosition The position in screen-space of the sprite
	 * \param flipped True if the sprite should be flipped along the vertical axis
	 * \param alpha The overloaded alpha channel
	 */
	void render_sprite(HAPISPACE::BYTE* screen, HAPISPACE::BYTE*   spriteSheetData, const int spriteSheetIndex,
	                   const Vector2    spritePosition, const bool flipped, const short       alpha)
	{
		if (flipped)
		{
			sprite_sheet::flip(spriteSheetData, spriteSheetIndex);
		}

		HAPISPACE::BYTE* spriteStart{
			spriteSheetData + static_cast<uint64_t>(
				constants::k_spriteSheetCellSize * constants::k_spriteSheetCellSize * 4) * spriteSheetIndex
		};

		sprite_sheet::clip_blit(screen, spriteStart, spriteSheetIndex, spritePosition, alpha);

		// works on the entire spritesheet so we need to flip back
		if (flipped)
		{
			sprite_sheet::flip(spriteSheetData, spriteSheetIndex);
		}
	}
}
