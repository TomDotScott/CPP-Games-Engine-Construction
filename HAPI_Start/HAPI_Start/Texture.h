#pragma once
#include <HAPI_lib.h>
#include "Vector.h"

class Texture
{
public:
	explicit Texture(const std::string& _fileName, Vector2 _position, bool _transparency = false);
	void Render(HAPISPACE::BYTE* _screen) const;
	
private:
	HAPISPACE::BYTE* m_textureData;
	int m_width;
	int m_height;
	Vector2 m_position;
	bool m_hasAlphaChannel;

	void FastBlit(HAPISPACE::BYTE* _screen) const;
	void AlphaBlit(HAPISPACE::BYTE* _screen) const;
};
