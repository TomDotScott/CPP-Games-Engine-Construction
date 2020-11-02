#include "Texture.h"

#include "../Utilities/Constants.h"

Texture::Texture() : m_textureData(), m_size(64, 64) {
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

void Texture::Render(HAPISPACE::BYTE* screen, const Vector2 position) const {
	if (m_textureData) {
		ClipBlit(screen, position);
	} else {
		HAPISPACE::HAPI_TColour* horridPink = &HAPISPACE::HAPI_TColour::HORRID_PINK;
		for (int x = static_cast<int>(position.x); x < (static_cast<int>(position.x + m_size.x)); x++) {
			for (int y = static_cast<int>(position.y); y < (static_cast<int>(position.y + m_size.y)); y++) {
				const int offset = (x + y * constants::k_screenWidth) * 4;
				memcpy(screen + offset, horridPink, 4);
			}
		}
	}
}

Vector2 Texture::GetSize() const {
	return{ m_size };
}

void Texture::AlphaBlit(HAPISPACE::BYTE* screen, const Vector2 position) const {
	HAPISPACE::BYTE* screenStart{
		screen + (static_cast<int>(position.y) * constants::k_screenWidth + static_cast<int>(position.x)) * 4
	};
	HAPISPACE::BYTE* textureStart{ m_textureData };

	const int increment{ constants::k_screenWidth * 4 - static_cast<int>(m_size.x) * 4 };

	for (int x = 0; x < static_cast<int>(m_size.x); x++) {
		for (int y = 0; y < static_cast<int>(m_size.y); y++) {
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

void Texture::ClipBlit(HAPISPACE::BYTE* screen, Vector2 position) const {
	const BoundsRectangle screenBounds{
		{ 0, 0 },
		{ constants::k_screenWidth, constants::k_screenHeight }
	};
	const BoundsRectangle objectBounds{
		position,
		{position.x + m_size.x, position.y + m_size.y}
	};
	auto tempRect = objectBounds;
	// If not completely offscreen, we need to render
	if(!tempRect.CompletelyOutside(screenBounds)) {
		// If not completely onscreen, we need to clip
		if(!tempRect.CompletelyInside(screenBounds)) {
			// Clip the rectangle
			tempRect.ClipToBound(screenBounds);
			// Translate the temporary Rectangle
			auto temp = position;
			tempRect.Translate(-temp.x, -temp.y);
			// Clamp the values
			temp.x = std::max(temp.x, 0.f);
			temp.y = std::max(temp.y, 0.f);

			// Find the start position of the screen for the texture 
			auto* screenPtr = screen + (static_cast<int>(temp.x + temp.y * screenBounds.GetSize().x)) * 4;
			// Find the start position of the texture
			auto* texturePtr = m_textureData + static_cast<int>((tempRect.GetTopLeft().x + tempRect.GetTopLeft().y * objectBounds.GetSize().x)) * 4;

			for (int y = 0; y < tempRect.GetSize().y; y++) {
				for (int x = 0; x < tempRect.GetSize().x; x++) {
					const auto red = texturePtr[0];
					const auto green = texturePtr[1];
					const auto blue = texturePtr[2];
					const auto alpha = texturePtr[3];

					if (alpha == 255) {
						//screen = texture
						screenPtr[0] = red;
						screenPtr[1] = green;
						screenPtr[2] = blue;
					} else if (alpha > 0) {
						screenPtr[0] = screenPtr[0] + ((alpha * (red - screenPtr[0])) >> 8);
						screenPtr[1] = screenPtr[1] + ((alpha * (green - screenPtr[1])) >> 8);
						screenPtr[2] = screenPtr[2] + ((alpha * (blue - screenPtr[2])) >> 8);
					}

					//Move texture pointer to next line
					screenPtr += 4;
					//Move screen pointer to the next line
					texturePtr += 4;
				}
				texturePtr += static_cast<int>((screenBounds.GetSize().x - tempRect.GetSize().x)) * 4;
				screenPtr += static_cast<int>((objectBounds.GetSize().x - tempRect.GetSize().x)) * 4;
			}
		} else {
			AlphaBlit(screen, position);
		}
	}
}
