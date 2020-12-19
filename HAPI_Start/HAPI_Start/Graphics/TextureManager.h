#pragma once
#include <HAPI_lib.h>
#include <unordered_map>
#include "../Design Patterns/Singleton.h"
#include "../Utilities/Constants.h"
#include "../Utilities/Vector.h"

class TextureManager : public Singleton<TextureManager>
{
	friend class Singleton<TextureManager>;
public:
	// Non-Copyable and Non-moveable
	TextureManager(const TextureManager& other) = delete;
	TextureManager(TextureManager&& other) noexcept = delete;

	TextureManager& operator=(const TextureManager& other) = delete;
	TextureManager& operator=(TextureManager&& other) noexcept = delete;
	
	void Initialise();
	void ClearScreen(HAPISPACE::HAPI_TColour col) const;
	void ClearScreen() const;

	void SetPixel(int x, int y, HAPISPACE::HAPI_TColour colour) const;
	void SetPixel(int x, int y, int value) const;

	bool CreateTexture(const std::string& filename, const std::string& name);
	void DrawTexture(const std::string& name, Vector2 position, const bool flipped = false);

	bool CreateSpriteSheet(const std::string& filename);
	bool CreateSprite(const std::string& spriteName, int spriteLocation);
	bool CreateSprite(const std::string& spriteName);
	void DrawSprite(const std::string& name, Vector2 position, bool flipped = false, short alpha = 255);

private:
	struct Texture
	{
		explicit Texture(const std::string& fileName) :
			textureData(),
			size(constants::k_spriteSheetCellSize, constants::k_spriteSheetCellSize)
		{
			int textureWidth{ 0 };
			int textureHeight{ 0 };
			if (!HAPI.LoadTexture(fileName, &textureData, textureWidth, textureHeight))
			{
				const std::string message{ "The file: " + fileName + " was not located. Check the file path or the filename." };
				HAPI.UserMessage(message, "File not found");
			}

			size = { static_cast<float>(textureWidth), static_cast<float>(textureHeight) };
		}

		~Texture()
		{
			delete[] textureData;
		}

		// Non-copyable and Non-moveable
		Texture(const Texture& other) = delete;
		Texture(Texture&& other) noexcept = delete;

		Texture& operator=(const Texture& other) = delete;
		Texture& operator=(Texture&& other) noexcept = delete;

		Vector2 GetSize() const
		{
			return size;
		}

		HAPISPACE::BYTE* textureData;
		Vector2 size;
	};

	
	HAPISPACE::BYTE* m_screen;
	std::unordered_map<std::string, Texture*> m_textureBuffer;

	std::unordered_map<std::string, int> m_spriteSheetLocations;
	Texture* m_spriteSheet;
	int m_nextSpritesheetLocation;
	
	TextureManager();
	~TextureManager();
};
