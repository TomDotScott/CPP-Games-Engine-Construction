#pragma once
#include <HAPI_lib.h>
#include "Vector.h"

struct GlobalBounds {
	GlobalBounds(Vector2 tl, Vector2 tr, Vector2 bl, Vector2 br) :
		m_topLeft(tl),
		m_topRight(tr),
		m_bottomLeft(bl),
		m_bottomRight(br) {
	};
	Vector2 m_topLeft;
	Vector2 m_topRight;
	Vector2 m_bottomLeft;
	Vector2 m_bottomRight;
};


class Texture {
public:
	explicit Texture(const std::string& _fileName, Vector2 _position);
	void Render(HAPISPACE::BYTE* _screen) const;
	void SetPosition(const Vector2 _pos);
	Vector2 GetSize() const;
	GlobalBounds GetGlobalBounds() const;
private:
	HAPISPACE::BYTE* m_textureData;
	int m_width;
	int m_height;
	Vector2 m_position;

	void AlphaBlit(HAPISPACE::BYTE* _screen) const;
};
