#pragma once
#include <HAPI_lib.h>
#include <unordered_map>
#include "../Design Patterns/Singleton.h"
#include "../Utilities/Constants.h"
#include "../Utilities/Vector.h"
#include "Texture.h"

class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	// Non-Copyable and Non-moveable
	TextureManager(const TextureManager& other) = delete;
	TextureManager(TextureManager&& other) noexcept = delete;

	TextureManager& operator=(const TextureManager& other) = delete;
	TextureManager& operator=(TextureManager&& other) noexcept = delete;
	
	void Initialise(HAPISPACE::BYTE* screenPtr);
	
	void ClearScreen(HAPISPACE::HAPI_TColour col) const;
	void ClearScreen() const;

	void SetPixel(int x, int y, HAPISPACE::HAPI_TColour colour) const;
	void SetPixel(int x, int y, int value) const;

	bool CreateTexture(const std::string& filename, const std::string& name);
	bool RemoveTexture(const std::string& name);
	void DrawTexture(const std::string& name, Vector2 position, const bool flipped = false);

	bool CreateSpriteSheet(const std::string& filename);
	bool CreateSprite(const std::string& spriteName, int spriteLocation);
	bool CreateSprite(const std::string& spriteName);
	void DrawSprite(const std::string& name, Vector2 position, bool flipped = false, short alpha = 255);

private:	
	HAPISPACE::BYTE* m_screen;
	std::unordered_map<std::string, Texture*> m_textureBuffer;

	std::unordered_map<std::string, int> m_spriteSheetLocations;
	Texture* m_spriteSheet;
	int m_nextSpritesheetLocation;
};
