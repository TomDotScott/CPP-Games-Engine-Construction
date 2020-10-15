#include "Texture.h"
#include "Constants.h"

Texture::Texture(const std::string& _fileName, const Vector2 _position, const bool _transparency) :
	m_textureData(nullptr),
	m_width(0),
	m_height(0),
	m_position(_position),
	m_hasAlphaChannel(_transparency) {
	if (!HAPI.LoadTexture(_fileName, &m_textureData, m_width, m_height)) {
		std::cout << "FILE : " << _fileName << " NOT FOUND" << std::endl;
	}
}

void Texture::Render(HAPISPACE::BYTE* _screen) const {
	if (m_hasAlphaChannel) {
		AlphaBlit(_screen);
	} else {
		FastBlit(_screen);
	}
}

void Texture::FastBlit(HAPISPACE::BYTE* _screen) const {
	for (int newY{ 0 }; newY < m_height; newY++) {
		const int offset{ static_cast<int>((constants::k_screenWidth * (m_position.y + static_cast<float>(newY)) + m_position.x) * 4) };
		const int textureOffset{ m_height * newY * 4 };
		memcpy(_screen + offset, m_textureData + textureOffset, static_cast<size_t>(m_width * 4));
	}
}

void Texture::AlphaBlit(HAPISPACE::BYTE* _screen) const {
	HAPISPACE::BYTE* screenStart{
		_screen + (static_cast<int>(m_position.x) + static_cast<int>(m_position.y) * m_width) * 4
	};
	HAPISPACE::BYTE* textureStart{ m_textureData };

	const int endOfLineDestIncrement{ constants::k_screenWidth * 4 - m_width * 4 };

	for (int y{ 0 }; y < m_height; y++) {
		for (int x{ 0 }; x < m_width; x++) {
			const HAPISPACE::BYTE r{ textureStart[0] };
			const HAPISPACE::BYTE g{ textureStart[1] };
			const HAPISPACE::BYTE b{ textureStart[2] };
			const HAPISPACE::BYTE a{ textureStart[3] };



			// Normalise alpha channel
			const float alphaChannelModifier{ static_cast<float>(a) / 255.f };

			// apply the modifier to the R, G and B channels
			screenStart[0] = static_cast<unsigned char>(alphaChannelModifier * static_cast<float>(r) +
				static_cast<float>(screenStart[0]) * (1.0f - alphaChannelModifier)
				);

			screenStart[1] = static_cast<unsigned char>(alphaChannelModifier * static_cast<float>(g) +
				static_cast<float>(screenStart[1]) * (1.0f - alphaChannelModifier)
				);

			screenStart[2] = static_cast<unsigned char>(alphaChannelModifier * static_cast<float>(b) +
				static_cast<float>(screenStart[2]) * (1.0f - alphaChannelModifier)
				);

			screenStart += 4;
			textureStart += 4;

		}
		screenStart += endOfLineDestIncrement;
	}
}
