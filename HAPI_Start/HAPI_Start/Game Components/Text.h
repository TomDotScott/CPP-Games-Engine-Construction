#pragma once
#include <string>
#include <HAPI_lib.h>
#include "../Utilities/Vector.h"

class TextureManager;

class Text
{
public:
	explicit Text(std::string text, Vector2 position = Vector2::CENTRE);

	void Render(TextureManager& textureManager) const;

	std::string& GetString();
	void SetString(const std::string& text);
	void SetPosition(Vector2 position);

	Vector2 GetPosition() const;
	
private:
	std::string m_text;
	Vector2 m_position;
};
