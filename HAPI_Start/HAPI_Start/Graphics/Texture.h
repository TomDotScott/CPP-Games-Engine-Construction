#pragma once
#include <HAPI_lib.h>
#include "../Utilities/BoundsRectangle.h"
#include "../Utilities/Vector.h"

class Texture {
public:
	Texture();
	~Texture();
	bool Initialise(const std::string& fileName);
	void RenderTexture(HAPISPACE::BYTE* screen, Vector2 texturePosition) const;
	void RenderSprite(HAPISPACE::BYTE* screen, int spriteSheetIndex, int cellWidth, const Vector2 spritePosition) const;
	Vector2 GetSize() const;

private:
	HAPISPACE::BYTE* m_textureData;
	Vector2 m_size;

	void AlphaBlit(HAPISPACE::BYTE* screen, Vector2 position) const;
	void ClipBlit(HAPISPACE::BYTE* screen, Vector2 position) const;
};
