#include "Text.h"
#include <utility>

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
	HAPI.RenderText(static_cast<int>(m_position.x), static_cast<int>(m_position.y), m_colour, m_text, m_characterSize, m_style);
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
