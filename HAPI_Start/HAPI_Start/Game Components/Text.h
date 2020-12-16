#pragma once
#include <string>
#include <HAPI_lib.h>
#include "../Utilities/Vector.h"

class Text
{
public:
	explicit Text(std::string text, HAPISPACE::HAPI_TColour colour, Vector2 position = Vector2::CENTRE, int size = 32, HAPISPACE::HAPI_TextStyle style = HAPISPACE::eRegular);

	void Render() const;
	
	void SetString(const std::string& text);
	void SetPosition(Vector2 position);
	void SetColour(HAPISPACE::HAPI_TColour colour);
	void SetStyle(HAPISPACE::HAPI_TextStyle style);
	
private:
	std::string m_text;
	HAPISPACE::HAPI_TColour m_colour;
	Vector2 m_position;
	int m_characterSize;
	HAPISPACE::HAPI_TextStyle m_style;
	
};