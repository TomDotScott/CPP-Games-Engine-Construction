#pragma once
#include <HAPI_lib.h>
#include <unordered_map>
#include "../Design Patterns/Singleton.h"
#include "Texture.h"

class Graphics : public Singleton<Graphics> {
	friend class Singleton<Graphics>;
public:
	void ClearScreen(HAPISPACE::HAPI_TColour col) const;
	void ClearScreen() const;

	void SetPixel(int x, int y, HAPISPACE::HAPI_TColour colour) const;
	void SetPixel(int x, int y, int value) const;

	bool CreateTexture(const std::string& filename, const std::string& name);
	bool CreateSpriteSheet(const std::string& filename, unsigned short cellSize);

	void SetSpriteSheetLocation(const std::string& spriteName, Vector2 gridLocation);
	
	void DrawTexture(const std::string& name, Vector2 position);
	void DrawSprite(const std::string& name, Vector2& position);


private:
	HAPISPACE::BYTE* m_screen;
	std::unordered_map<std::string, Texture*> m_textureBuffer;

	std::unordered_map<std::string, Vector2> m_spriteSheetLocations;
	Texture* m_spriteSheet;
	unsigned short m_spriteSheetCellSize;
	
	Graphics();
	~Graphics();
};
