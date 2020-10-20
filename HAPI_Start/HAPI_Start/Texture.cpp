#include "Texture.h"
#include "Constants.h"

Texture::Texture(const std::string& _fileName, const Vector2 _position) :
	m_textureData(nullptr),
	m_width(0),
	m_height(0),
	m_position(_position) {
	if (!HAPI.LoadTexture(_fileName, &m_textureData, m_width, m_height)) {
		std::cout << "FILE : " << _fileName << " NOT FOUND" << std::endl;
	}
}

void Texture::Render(HAPISPACE::BYTE* _screen) const
{
	AlphaBlit(_screen);
}

void Texture::SetPosition(const Vector2 _pos) {
	m_position = _pos;
}

Vector2 Texture::GetSize() const {
	return{static_cast<float>(m_width), static_cast<float>(m_height)};
}

GlobalBounds Texture::GetGlobalBounds() const {
	// return { m_position, {m_position.x + m_width, m_position.y + m_height} };
	return {
		m_position,
		{ m_position.x + static_cast<float>(m_width), m_position.y },
		{ m_position.x, m_position.y + static_cast<float>(m_height)},
		{ m_position.x + static_cast<float>(m_width), m_position.y + static_cast<float>(m_height)}
	};
}

void Texture::AlphaBlit(HAPISPACE::BYTE* _screen) const {
	HAPISPACE::BYTE* screenStart{
		_screen + (static_cast<int>(m_position.x) + static_cast<int>(m_position.y) * constants::k_screenWidth) * 4
	};
	HAPISPACE::BYTE* textureStart{ m_textureData };

	const int increment{ constants::k_screenWidth * 4 - m_width * 4 };

	for (int x = 0; x < m_width; x++) {
		for (int y = 0; y < m_height; y++) {
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
