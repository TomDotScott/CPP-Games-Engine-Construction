#pragma once
#include <HAPI_lib.h>
#include "../Utilities/CollisionBox.h"
#include "../Utilities/Vector.h"

class Texture
{
public:
	Texture();
	~Texture();
	bool Initialise(const std::string& fileName);
	void RenderTexture(HAPISPACE::BYTE* screen, Vector2 texturePosition) const;
	void RenderSprite(HAPISPACE::BYTE* screen, int spriteSheetIndex, Vector2 spritePosition, bool flipped = false, short alpha = 255);
	Vector2 GetSize() const;

private:
	HAPISPACE::BYTE* m_textureData;
	Vector2 m_size;

	void FlipSprite(int spriteSheetLocation, const int width, const int height);
	void TextureAlphaBlit(HAPISPACE::BYTE* screen, Vector2 position) const;
	void TextureClipBlit(HAPISPACE::BYTE* screen, Vector2 position) const;
	void SpriteAlphaBlit(HAPISPACE::BYTE* screenStart, HAPISPACE::BYTE* spriteData, short alpha = 255) const;
	void SpriteClipBlit(HAPISPACE::BYTE* screenStart, HAPISPACE::BYTE* spriteData, int spriteSheetIndex, Vector2 position, short alpha = 255) const;

};
