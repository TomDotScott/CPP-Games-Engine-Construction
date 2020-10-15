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
	FastBlit(_screen);
}

void Texture::FastBlit(HAPISPACE::BYTE* _screen) const {
	for (int newY{ 0 }; newY < m_height; newY++) {
		const int offset{static_cast<int>((constants::k_screenWidth * (m_position.y + static_cast<float>(newY)) + m_position.x) * 4)};
		const int textureOffset{ m_height * newY * 4 };
		memcpy(_screen + offset, m_textureData + textureOffset, m_width * 4);
	}
}

void Texture::AlphaBlit(HAPISPACE::BYTE* _screen) const {
	return;
}
