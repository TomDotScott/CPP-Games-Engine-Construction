#pragma once
#include <string>
#include <HAPI_lib.h>
#include "../Utilities/Vector.h"

class TextureManager;

class Text
{
public:
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
