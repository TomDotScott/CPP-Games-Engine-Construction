#include "Graphics.h"
#include "../Utilities/Constants.h"

Graphics::Graphics() : m_screen(), m_textureBuffer(), m_spriteSheetLocations(), m_spriteSheet(), m_spriteSheetCellSize(64) {
	int width = constants::k_screenWidth;
	int height = constants::k_screenHeight;

	if (!HAPI.Initialise(width, height, "HAPI Pong!"))
		return;
	HAPI.SetShowFPS(true);
	m_screen = HAPI.GetScreenPointer();
}

Graphics::~Graphics() {
	for (auto& texture : m_textureBuffer) {
		delete texture.second;
	}
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
	auto* newTexture = new Texture();
	if (!newTexture->Initialise(filename)) {
		HAPI.UserMessage("Could not load the file: " + filename + "\nPlease check the spelling and try again", "Error Occured");
		delete newTexture;
		return false;
	}
	m_textureBuffer[name] = newTexture;
	return true;
}

bool Graphics::CreateSpriteSheet(const std::string& filename, unsigned short cellSize) {
	auto* newSprite = new Texture();
	if (!newSprite->Initialise(filename)) {
		HAPI.UserMessage("Could not load the file: " + filename + "\nPlease check the spelling and try again", "Error Occured");
		delete newSprite;
		return false;
	} else {
		m_spriteSheet = newSprite;
	}
	if ((static_cast<int>(m_spriteSheet->GetSize().x * m_spriteSheet->GetSize().y) % (cellSize * cellSize)) == 0) {
		m_spriteSheetCellSize = cellSize;
	}else {
		HAPI.UserMessage(std::to_string(cellSize) + " is an invalid cell size", "Error Occured");
	}
	return true;
}

void Graphics::SetSpriteSheetLocation(const std::string& spriteName, Vector2 gridLocation) {
	if(gridLocation.x >= 0 
		&& gridLocation.y >= 0 
		&& gridLocation.x <= m_spriteSheet->GetSize().x / m_spriteSheetCellSize 
		&& gridLocation.y <= m_spriteSheet->GetSize().y / m_spriteSheetCellSize) {
		if(!(m_spriteSheetLocations.find(spriteName) == m_spriteSheetLocations.end())) {
			HAPI.UserMessage("A sprite with the name " + spriteName + " already exists.", "Error Occured");
		} else {
			m_spriteSheetLocations[spriteName] = gridLocation;
		}
	}else {
		HAPI.UserMessage("Sprite Grid Location Not Valid", "Error Occured");
	}
}

void Graphics::DrawTexture(const std::string& name, const Vector2 position) {
	if (!m_textureBuffer.at(name)) {
		HAPI.UserMessage("Error: Can't draw the Texture " + name + "\nCheck the Spelling and try again.", "Error :(");
		return;
	}
	m_textureBuffer.at(name)->RenderTexture(m_screen, position);
}

void Graphics::DrawSprite(const std::string& name, Vector2& position) {
	if (m_spriteSheetLocations.find(name) == m_spriteSheetLocations.end()) {
		HAPI.UserMessage("Error: Can't draw the sprite " + name + "\nCheck the Spelling and try again.", "Error Occured");
		return;
	}else {
		m_spriteSheet->RenderSprite(m_screen, m_spriteSheetLocations[name], m_spriteSheetCellSize, position);
	}
}
