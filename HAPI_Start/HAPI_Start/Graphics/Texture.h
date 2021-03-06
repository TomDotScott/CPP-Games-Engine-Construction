﻿#pragma once
#include "../Utilities/Constants.h"
#include "../Utilities/Vector.h"

/**
 * \brief The Texture struct acts as a template for the texture data
 * used within the engine
 */
struct Texture
{
	/**
	 * \brief Constructs a Texture object
	 * \param fileName The file location of the texture, relative to
	 * the working directory
	 */
	explicit Texture(const std::string& fileName) :
		m_textureData(),
		m_size(constants::k_spriteSheetCellSize, constants::k_spriteSheetCellSize)
	{
		int textureWidth{0};
		int textureHeight{0};

		// Use HAPI to load the texture
		if (!HAPI.LoadTexture(fileName, &m_textureData, textureWidth, textureHeight))
		{
			const std::string message{
				"The file: " + fileName + " was not located. Check the file path or the filename."
			};
			HAPI.UserMessage(message, "File not found");
		}

		m_size = {static_cast<float>(textureWidth), static_cast<float>(textureHeight)};
	}

	~Texture()
	{
		delete[] m_textureData;
	}

	// Non-copyable and Non-moveable
	Texture(const Texture& other)     = delete;
	Texture(Texture&& other) noexcept = delete;

	Texture& operator=(const Texture& other)     = delete;
	Texture& operator=(Texture&& other) noexcept = delete;

	HAPISPACE::BYTE* m_textureData;
	Vector2          m_size;
};
