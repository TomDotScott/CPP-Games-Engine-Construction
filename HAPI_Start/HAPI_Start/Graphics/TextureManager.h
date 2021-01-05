#pragma once
#include <HAPI_lib.h>
#include <unordered_map>

#include "Texture.h"
#include "../Utilities/Vector.h"

/**
 * \brief The TextureManager holds the textures and spritesheets of the
 * game and renders them to the screen using the renderer namespace
 */
class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	// Non-Copyable and Non-moveable
	TextureManager(const TextureManager& other)     = delete;
	TextureManager(TextureManager&& other) noexcept = delete;

	TextureManager& operator=(const TextureManager& other)     = delete;
	TextureManager& operator=(TextureManager&& other) noexcept = delete;

	/**
	 * \brief Initialises the TextureManager
	 * \param screenPtr A pointer to the screen data
	 */
	void Initialise(HAPISPACE::BYTE* screenPtr);
	
	/**
	 * \brief Clears the screen to a specific colour
	 * \param col The colour the screen should be cleared to
	 */
	void ClearScreen(HAPISPACE::HAPI_TColour col) const;
	/**
	 * \brief A more efficient clearing of the screen. Clears
	 * to black
	 */
	void ClearScreen() const;
	
	/**
	 * \brief Opens the spritesheet file and creates a spritesheet object
	 * \param filename The file location of the spritesheet, relative to the
	 * working directory
	 * \return True if the spritesheet was loaded successfully
	 */
	bool CreateSpriteSheet(const std::string& filename);
	/**
	 * \brief Creates a sprite using the spritesheet
	 * \param spriteName The identifier in the spritesheetbuffer
	 * \param spriteLocation The index of the sprite on the spritesheet
	 * \return True if the sprite was created successfully
	 */
	bool CreateSprite(const std::string& spriteName, int spriteLocation);
	/**
	 * \brief Creates a sprite using the spritesheet at the next available
	 * location
	 * \param spriteName The identifier in the spritesheetbuffer
	 * \return True if the sprite was created successfully
	 */
	bool CreateSprite(const std::string& spriteName);
	/**
	 * \brief Creates a texture object and adds to the texturebuffer
	 * \param filename The file location of the texture, relative to the
	 * working directory
	 * \param name The identifier for the texture in the texturebuffer
	 * \return True if the texture was created successfully
	 */
	bool CreateTexture(const std::string& filename, const std::string& name);
	
	/**
	 * \brief Sets a desired pixel to a specific colour
	 * \param x The x position of the pixel
	 * \param y The y position of the pixel
	 * \param colour The colour the pixel should be set to
	 */
	void SetPixel(int x, int y, HAPISPACE::HAPI_TColour colour) const;
	void SetPixel(int x, int y, unsigned char) const;
	
	/**
	 * \brief Removes the texture from the texturebuffer
	 * \param name The identifier of the texture in the
	 * texturebuffer
	 * \return True if the texture was removed successfully
	 */
	bool RemoveTexture(const std::string& name);
	
	/**
	 * \brief Renders a texture to the screen
	 * \param name The identifier of the texture to be drawn
	 * \param position The position of the texture in screen-space
	 * \param flipped True if the texture should be flipped along the
	 * vertical axis
	 */
	void DrawTexture(const std::string& name, Vector2 position, bool flipped = false);
	/**
	 * \brief Renders a sprite to the screen
	 * \param name The identifier of the sprite to be drawn
	 * \param position The position of the sprite in screen-space
	 * \param flipped True if the sprite should be flipped along the vertical
	 * axis
	 * \param alpha A value between 0 and 255 to overload the alpha channel of the
	 * sprite to give it transparency
	 */
	void DrawSprite(const std::string& name, Vector2 position, bool flipped = false, short alpha = 255);

private:
	HAPISPACE::BYTE*                          m_screen;
	std::unordered_map<std::string, Texture*> m_textureBuffer;

	std::unordered_map<std::string, int> m_spriteSheetLocations;
	Texture*                             m_spriteSheet;
	int                                  m_nextSpritesheetLocation;
};
