#include "Texture.h"
#include "../Utilities/Constants.h"

Texture::Texture() : m_textureData(), m_size(constants::k_spriteSheetCellWith, constants::k_spriteSheetCellWith) {
}

Texture::~Texture() {
	delete[] m_textureData;
}

bool Texture::Initialise(const std::string& fileName) {
	int textureWidth{ 0 };
	int textureHeight{ 0 };
	if (!HAPI.LoadTexture(fileName, &m_textureData, textureWidth, textureHeight)) {
		const std::string message{ "The file: " + fileName + " was not located. Check the file path or the filename." };
		HAPI.UserMessage(message, "File not found");
		return false;
	}

	m_size = { static_cast<float>(textureWidth), static_cast<float>(textureHeight) };
	return true;
}

void Texture::RenderTexture(HAPISPACE::BYTE* screen, const Vector2 texturePosition) const {
	TextureClipBlit(screen, texturePosition);
}

void Texture::RenderSprite(HAPISPACE::BYTE* screen, const int spriteSheetIndex, const Vector2 spritePosition) const {
	HAPISPACE::BYTE* screenStart{
		screen + (static_cast<int>(spritePosition.x) + static_cast<int>(spritePosition.y) * constants::k_screenWidth) * 4
	};

	HAPISPACE::BYTE* textureStart{
		m_textureData + (constants::k_spriteSheetCellWith * constants::k_spriteSheetCellWith * 4) * spriteSheetIndex
	};

	/*SpriteAlphaBlit(screenStart, textureStart, cellWidth, spritePosition);*/
	SpriteClipBlit(screenStart, textureStart, spriteSheetIndex, spritePosition);

}

Vector2 Texture::GetSize() const {
	return{ m_size };
}

void Texture::TextureAlphaBlit(HAPISPACE::BYTE* screen, const Vector2 position) const {
	HAPISPACE::BYTE* screenStart{
		screen + (static_cast<int>(position.y) * constants::k_screenWidth + static_cast<int>(position.x)) * 4
	};
	HAPISPACE::BYTE* textureStart{ m_textureData };

	const int increment{ constants::k_screenWidth * 4 - static_cast<int>(m_size.x) * 4 };

	for (int y = 0; y < static_cast<int>(m_size.y); y++) {
		for (int x = 0; x < static_cast<int>(m_size.x); x++) {
			const HAPISPACE::BYTE a{ textureStart[3] };
			// Only draw pixels if needed
			if (a > 0) {
				// Fast blit if no alpha
				if (a == 255) {
					memcpy(screenStart, textureStart, 4);
				} else {
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

void Texture::TextureClipBlit(HAPISPACE::BYTE* screen, Vector2 position) const {
	// BoundsRectangle takes in coordinates for the top left and bottom right
	// My Vector2 class defaults to zeros
	Vector2 temp = position;
	const BoundsRectangle textureBounds({}, m_size);

	const BoundsRectangle screenBounds({}, { constants::k_screenWidth, constants::k_screenHeight });

	// Create a copy to clip with 
	BoundsRectangle clippedRect(textureBounds);

	//Translate to screen space
	clippedRect.Translate(temp.x, temp.y);

	// If the object is onscreen...
	if (!clippedRect.IsCompletelyOutside(screenBounds)) {
		// If the object is completely onscreen then alphablit...
		if (clippedRect.IsCompletelyInside(screenBounds)) {
			TextureAlphaBlit(screen, position);
		} else {
			// we must be offscreen...
			//Clip against screen
			clippedRect.ClipTo(screenBounds);

			clippedRect.Translate(-temp.x, -temp.y);

			//Clamping to negative
			position.x = std::max(0.f, temp.x);
			position.y = std::max(0.f, temp.y);

			// Calculate offsets and starting points for screen and texture....
			const int screenOffset{
				(static_cast<int>(position.x) + static_cast<int>(position.y) * static_cast<int>(screenBounds.GetSize().x)) * 4
			};

			HAPISPACE::BYTE* screenPtr = screen + screenOffset;

			const int textureOffset{
				(static_cast<int>(clippedRect.TOP_LEFT.x) + static_cast<int>(clippedRect.TOP_LEFT.y) * static_cast<int>(textureBounds.GetSize().x)) * 4
			};

			HAPISPACE::BYTE* texPtr = m_textureData + textureOffset;

			// Calculate the increment up here rather than in the loop since it doesn't change...
			const int screenInc{
				(static_cast<int>(screenBounds.GetSize().x) - static_cast<int>(clippedRect.GetSize().x)) * 4
			};

			const int texInc{
				(static_cast<int>(textureBounds.GetSize().x) - static_cast<int>(clippedRect.GetSize().x)) * 4
			};

			// Start blitting...
			for (int y = 0; y < clippedRect.GetSize().y; y++) {
				for (int x = 0; x < clippedRect.GetSize().x; x++) {
					const HAPISPACE::BYTE alpha = texPtr[3];
					// Fully opaque
					if (alpha == 255) {
						memcpy(screenPtr, texPtr, 4);
					} else if (alpha > 0) { // Has alpha channel
						const HAPISPACE::BYTE red = texPtr[0];
						const HAPISPACE::BYTE green = texPtr[1];
						const HAPISPACE::BYTE blue = texPtr[2];

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

void Texture::SpriteAlphaBlit(HAPISPACE::BYTE* screenStart, HAPISPACE::BYTE* spriteData, const Vector2 position) const {
	const int screenInc{ constants::k_screenWidth * 4 - constants::k_spriteSheetCellWith * 4 };

	const int spriteInc{ static_cast<int>(m_size.x) * 4 - constants::k_spriteSheetCellWith * 4 };

	for (int y = 0; y < constants::k_spriteSheetCellWith; y++) {
		for (int x = 0; x < constants::k_spriteSheetCellWith; x++) {
			const HAPISPACE::BYTE a{ spriteData[3] };
			// Only draw pixels if needed
			if (a > 0) {
				// Fast blit if no alpha
				if (a == 255) {
					memcpy(screenStart, spriteData, 4);
				} else {
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
		spriteData += spriteInc;
	}
}

void Texture::SpriteClipBlit(HAPISPACE::BYTE* screenStart, HAPISPACE::BYTE* spriteData, const int spriteSheetIndex, Vector2 position) const {
	// BoundsRectangle takes in coordinates for the top left and bottom right
	// My Vector2 class defaults to zeros
	const Vector2 temp = position;
	const BoundsRectangle spriteBounds({}, { static_cast<float>(constants::k_spriteSheetCellWith), static_cast<float>(constants::k_spriteSheetCellWith) });

	const BoundsRectangle screenBounds({}, { constants::k_screenWidth, constants::k_screenHeight });

	// Create a copy to clip with 
	BoundsRectangle clippedRect(spriteBounds);

	//Translate to screen space
	clippedRect.Translate(temp.x, temp.y);

	// If the object is onscreen...
	if (!clippedRect.IsCompletelyOutside(screenBounds)) {
		// If the object is completely onscreen then alphablit...
		if (clippedRect.IsCompletelyInside(screenBounds)) {
			SpriteAlphaBlit(screenStart, spriteData, position);
		} else {
			// we must be offscreen...
			//Clip against screen
			clippedRect.ClipTo(screenBounds);

			clippedRect.Translate(-temp.x, -temp.y);

			//Clamping to negative
			position.x = std::max(0.f, temp.x);
			position.y = std::max(0.f, temp.y);

			// Calculate the increment up here rather than in the loop since it doesn't change...
			const int screenInc{static_cast<int>(screenBounds.GetSize().x) * 4 - static_cast<int>(clippedRect.GetSize().x) * 4 };


			const int spriteOffset{
				(static_cast<int>(clippedRect.TOP_LEFT.y) + static_cast<int>(clippedRect.TOP_LEFT.x) * constants::k_spriteSheetCellWith) * 4
			};

			HAPISPACE::BYTE* spritePtr = spriteData + spriteOffset;
			
			const int spriteInc {
				((constants::k_spriteSheetCellWith - static_cast<int>(clippedRect.GetSize().x))) * 4
			};

			// Start blitting...
			for (int y = 0; y < clippedRect.GetSize().y; y++) {
				for (int x = 0; x < clippedRect.GetSize().x; x++) {
					const HAPISPACE::BYTE alpha = spritePtr[3];
					// Fully opaque
					if (alpha == 255) {
						memcpy(screenStart, spritePtr, 4);
					} else if (alpha > 0) { // Has alpha channel
						const HAPISPACE::BYTE red = spritePtr[0];
						const HAPISPACE::BYTE green = spritePtr[1];
						const HAPISPACE::BYTE blue = spritePtr[2];

						screenStart[0] = screenStart[0] + ((alpha * (red - screenStart[0])) >> 8);
						screenStart[1] = screenStart[1] + ((alpha * (green - screenStart[1])) >> 8);
						screenStart[2] = screenStart[2] + ((alpha * (blue - screenStart[2])) >> 8);
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
