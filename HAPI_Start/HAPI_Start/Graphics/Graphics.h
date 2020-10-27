#pragma once
#include <HAPI_lib.h>
#include <unordered_map>
#include "Texture.h"

class Graphics {
public:
	~Graphics();
	static Graphics& GetInstance();
	
	void ClearScreen(HAPISPACE::HAPI_TColour _col) const;
	void ClearScreen() const;
	
	void SetPixel(int _x, int _y, HAPISPACE::HAPI_TColour _colour) const;
	void SetPixel(int _x, int _y, int _value) const;
	
	bool CreateTexture(const std::string& filename, const std::string& name);
	void DrawTexture(const std::string& _name, Vector2 _position);
	
private:
	Graphics();
	HAPISPACE::BYTE* m_screen;

	std::unordered_map<std::string, Texture*> m_textureMap;
	static Graphics* m_instance;
};
