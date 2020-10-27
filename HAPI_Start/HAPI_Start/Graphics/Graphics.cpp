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

void Graphics::ClearScreen(HAPISPACE::HAPI_TColour col) const {
	for (int i = 0; i < constants::k_screenWidth * constants::k_screenHeight; i++) {
		memcpy(m_screen + 4 * i, &col, 4);
	}
}

void Graphics::ClearScreen() const {
	memset(m_screen, 0, static_cast<size_t>(constants::k_screenWidth * constants::k_screenHeight * 4));
}

void Graphics::SetPixel(const int x, const int y, const HAPISPACE::HAPI_TColour colour) const {
	m_screen[constants::k_screenWidth * x + y] = colour.red;
	m_screen[constants::k_screenWidth * x + y + 1] = colour.green;
	m_screen[constants::k_screenWidth * x + y + 2] = colour.blue;
	m_screen[constants::k_screenWidth * x + y + 3] = colour.alpha;
}

void Graphics::SetPixel(const int x, const int y, const int value) const {
	m_screen[constants::k_screenWidth * x + y] = value;
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

void Graphics::DrawTexture(const std::string& name, const Vector2 position) {
	if (!m_textureMap.at(name)) {
		HAPI.UserMessage("Error: Can't draw the sprite " + name + "\nCheck the Spelling and try again.", "Error :(");
		return;
	}
	m_textureMap.at(name)->Render(m_screen, position);
}