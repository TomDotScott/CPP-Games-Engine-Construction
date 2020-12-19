#include "Text.h"
#include <utility>

#include "../Graphics/TextureManager.h"

Text::Text(std::string text, const HAPISPACE::HAPI_TColour colour, const Vector2 position, const int size, const HAPISPACE::HAPI_TextStyle style) :
	m_text(std::move(text)),
	m_colour(colour),
	m_position(position),
	m_characterSize(size),
	m_style(style)
{
}

void Text::Render() const
{
	/*HAPI.RenderText(
		static_cast<int>(m_position.x),
		static_cast<int>(m_position.y),
		m_colour,
		m_text,
		m_characterSize,
		m_style
	);*/

	float xOffset{ 0.f };
	for (auto& character : m_text)
	{
		std::string spriteName;
		switch (character)
		{
		case 'O':
		case 'o':
		case '0':
			spriteName = "UI_0";
			break;
		case '1':
			spriteName = "UI_1";
			break;
		case '2':
			spriteName = "UI_2";
			break;
		case '3':
			spriteName = "UI_3";
			break;
		case '4':
			spriteName = "UI_4";
			break;
		case '5':
			spriteName = "UI_5";
			break;
		case '6':
			spriteName = "UI_6";
			break;
		case '7':
			spriteName = "UI_7";
			break;
		case '8':
			spriteName = "UI_8";
			break;
		case '9':
			spriteName = "UI_9";
			break;
		case 'C':
		case 'c':
			spriteName = "UI_C";
			break;
		case 'D':
		case 'd':
			spriteName = "UI_D";
			break;
		case 'E':
		case 'e':
			spriteName = "UI_E";
			break;
		case 'I':
		case 'i':
			spriteName = "UI_I";
			break;
		case 'L':
		case 'l':
			spriteName = "UI_L";
			break;		
		case 'R':
		case 'r':
			spriteName = "UI_R";
			break;
		case 'S':
		case 's':
			spriteName = "UI_S";
			break;
		case 'T':
		case 't':
			spriteName = "UI_T";
			break;
		case 'V':
		case 'v':
			spriteName = "UI_V";
			break;
		case 'W':
		case 'w':
			spriteName = "UI_W";
			break;
		default:
			xOffset += 28.f;
			continue;
			break;
		}

		TextureManager::GetInstance().DrawSprite(spriteName, { m_position.x + xOffset, m_position.y });
		xOffset += 28.f;
	}
}

std::string& Text::GetString()
{
	return m_text;
}

void Text::SetString(const std::string& text)
{
	m_text = text;
}

void Text::SetPosition(const Vector2 position)
{
	m_position = position;
}

void Text::SetColour(const HAPISPACE::HAPI_TColour colour)
{
	m_colour = colour;
}

void Text::SetStyle(const HAPISPACE::HAPI_TextStyle style)
{
	m_style = style;
}
