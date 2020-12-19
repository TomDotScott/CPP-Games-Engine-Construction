#pragma once

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