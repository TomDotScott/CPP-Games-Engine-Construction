#pragma once
#include <HAPI_lib.h>
#include <string>

#include "../Graphics/TextureManager.h"
#include "../Utilities/Vector.h"

/**
 * \brief Text objects hold string data and render to the screen
 * via each character in the string being a sprite in the spritesheet
 */
class Text
{
public:
	/**
	 * \brief Constructs a text object
	 * \param text The initial string value of the object
	 * \param position The initial position of the text object
	 */
	explicit Text(std::string text, const Vector2& position = Vector2::CENTRE);

	void Render(TextureManager& textureManager) const;

	std::string& GetString();
	Vector2      GetPosition() const;

	void SetString(const std::string& text);
	void SetPosition(const Vector2& position);

private:
	std::string m_text;
	Vector2     m_position;
};
