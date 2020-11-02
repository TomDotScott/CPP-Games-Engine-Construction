#pragma once
#include <HAPI_lib.h>
#include "../Utilities/Vector.h"

class Texture {
public:
	Texture();
	~Texture();
	bool Initialise(const std::string& fileName);
	void Render(HAPISPACE::BYTE* screen, Vector2 position) const;
	Vector2 GetSize() const;

private:
	HAPISPACE::BYTE* m_textureData;
	Vector2 m_size;

	void AlphaBlit(HAPISPACE::BYTE* screen, Vector2 position) const;
	void ClipBlit(HAPISPACE::BYTE* screen, Vector2 position);
};
