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

void Texture::Render(HAPISPACE::BYTE* _screen, const Vector2 _position) const {
	if (m_textureData) {
		AlphaBlit(_screen, _position);
	} else {
		HAPISPACE::HAPI_TColour* horridPink = &HAPISPACE::HAPI_TColour::HORRID_PINK;
		for (int x = static_cast<int>(_position.x); x < (static_cast<int>(_position.x + m_size.x)); x++) {
			for (int y = static_cast<int>(_position.y); y < (static_cast<int>(_position.y + m_size.y)); y++) {
				const int offset = (x + y * constants::k_screenWidth) * 4;
				memcpy(_screen + offset, horridPink, 4);
			}
		}
	}
}

Vector2 Texture::GetSize() const {
	return{ m_size };
}

void Texture::AlphaBlit(HAPISPACE::BYTE* _screen, const Vector2 _position) const {
	HAPISPACE::BYTE* screenStart{
		_screen + (static_cast<int>(_position.y) * constants::k_screenWidth + static_cast<int>(_position.x)) * 4
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
