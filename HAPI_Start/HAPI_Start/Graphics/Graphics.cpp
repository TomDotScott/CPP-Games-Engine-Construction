#include "Graphics.h"
#include "../Utilities/Constants.h"

Graphics* Graphics::m_instance{ nullptr };

Graphics& Graphics::GetInstance() {
	if (m_instance == nullptr) {
		m_instance = new Graphics();
	}
	return *m_instance;
}

Graphics::Graphics() : m_screen(), m_textureMap() {
	int width = constants::k_screenWidth;
	int height = constants::k_screenHeight;

	if (!HAPI.Initialise(width, height, "HAPI Pong!"))
		return;

	m_screen = HAPI.GetScreenPointer();
}

Graphics::~Graphics() {
	for (auto& texture : m_textureMap) {
		delete texture.second;
	}
	delete m_instance;
}

void Graphics::ClearScreen(HAPISPACE::HAPI_TColour _col) const {
	for (int i = 0; i < constants::k_screenWidth * constants::k_screenHeight; i++) {
		memcpy(m_screen + 4 * i, &_col, 4);
	}
}

void Graphics::ClearScreen() const {
	memset(m_screen, 0, static_cast<size_t>(constants::k_screenWidth * constants::k_screenHeight * 4));
}

void Graphics::SetPixel(const int _x, const int _y, const HAPISPACE::HAPI_TColour _colour) const {
	m_screen[constants::k_screenWidth * _x + _y] = _colour.red;
	m_screen[constants::k_screenWidth * _x + _y + 1] = _colour.green;
	m_screen[constants::k_screenWidth * _x + _y + 2] = _colour.blue;
	m_screen[constants::k_screenWidth * _x + _y + 3] = _colour.alpha;
}

void Graphics::SetPixel(const int _x, const int _y, const int _value) const {
	m_screen[constants::k_screenWidth * _x + _y] = _value;
}


bool Graphics::CreateTexture(const std::string& filename, const std::string& name) {
	auto* newSprite = new Texture();
	if (!newSprite->Initialise(filename)) {
		delete newSprite;
		return false;
	}
	m_textureMap[name] = newSprite;
	return true;
}

void Graphics::DrawTexture(const std::string& _name, const Vector2 _position) {
	if (!m_textureMap.at(_name)) {
		HAPI.UserMessage("Error: Can't draw the sprite " + _name + "\nCheck the Spelling and try again.", "Error :(");
		return;
	}
	m_textureMap.at(_name)->Render(m_screen, _position);
}