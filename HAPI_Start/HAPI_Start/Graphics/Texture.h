#pragma once
#include <HAPI_lib.h>
#include "../Utilities/Vector.h"

class Texture {
public:
	Texture();
	~Texture();
	bool Initialise(const std::string& fileName);
	void Render(HAPISPACE::BYTE* _screen, Vector2 _position) const;
	Vector2 GetSize() const;
	
private:
	HAPISPACE::BYTE* m_textureData;
	Vector2 m_size;

	void AlphaBlit(HAPISPACE::BYTE* _screen, Vector2 _position) const;
};
