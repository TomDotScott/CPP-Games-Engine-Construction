#pragma once
#include <HAPI_lib.h>
#include "Vector.h"

class Texture {
public:
	explicit Texture(const std::string& _fileName, Vector2 _position);
	void Render(HAPISPACE::BYTE* _screen) const;
	void SetPosition(const Vector2 _pos);
	
	std::pair<Vector2, Vector2> GetGlobalBounds() const;
private:
	HAPISPACE::BYTE* m_textureData;
	int m_width;
	int m_height;
	Vector2 m_position;
	
	void AlphaBlit(HAPISPACE::BYTE* _screen) const;
};
