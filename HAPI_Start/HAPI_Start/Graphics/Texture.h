#pragma once
#include <HAPI_lib.h>
#include "../Utilities/BoundsRectangle.h"
#include "../Utilities/Vector.h"

class Texture {
public:
	Texture();
	~Texture();
	bool Initialise(const std::string& fileName);
	void RenderTexture(HAPISPACE::BYTE* screen, const Vector2 texturePosition) const;
	void RenderSprite(HAPISPACE::BYTE* screen, const int spriteSheetIndex, const Vector2 spritePosition) const;
	Vector2 GetSize() const;

private:
	HAPISPACE::BYTE* m_textureData;
	Vector2 m_size;

	void TextureAlphaBlit(HAPISPACE::BYTE* screen, Vector2 position) const;
	void TextureClipBlit(HAPISPACE::BYTE* screen, Vector2 position) const;
	void SpriteAlphaBlit(HAPISPACE::BYTE* screenStart, HAPISPACE::BYTE* spriteData, const Vector2 position) const;
	void SpriteClipBlit(HAPISPACE::BYTE* screenStart, HAPISPACE::BYTE* spriteData, const int spriteSheetIndex, Vector2 position) const;
	
};
