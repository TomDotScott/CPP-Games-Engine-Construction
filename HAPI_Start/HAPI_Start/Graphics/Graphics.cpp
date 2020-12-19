#include "Graphics.h"
#include "../Utilities/Constants.h"

Graphics::Graphics() :
	m_screen(),
	m_spriteSheetLocations(),
	m_spriteSheet(),
	m_nextSpritesheetLocation(0)
{
	int width = constants::k_screenWidth;
	int height = constants::k_screenHeight;

	if (!HAPI.Initialise(width, height, "Nano's Adventure"))
	{
		return;
	}
	HAPI.SetShowFPS(true);
	
	m_screen = HAPI.GetScreenPointer();
}

Graphics::~Graphics()
{
	for (auto& texture : m_textureBuffer)
	{
		delete texture.second;
	}
}

void Graphics::Initialise()
{
	//-----------------------TEXTURES-------------------------
	CreateTexture("Res/Graphics/Level1_Background.tga", "Level1_Background");
	CreateTexture("Res/Graphics/Level2_Background.tga", "Level2_Background");
	CreateSpriteSheet("Res/Graphics/GameSpriteSheet.tga");

	// ----------------------SPRITESHEET-----------------------
	CreateSprite("Player_Small_Dead_Body");
	CreateSprite("Player_Small_Walk_Body_1");
	CreateSprite("Player_Small_Walk_Body_2");
	CreateSprite("Player_Small_Jump_Body_1");
	CreateSprite("Player_Small_Idle_Body_1");
	CreateSprite("Player_Small_Idle_Body_2");
	CreateSprite("Player_Dead_Top");
	CreateSprite("Player_Dead_Body");
	CreateSprite("Player_Walk_Top_1");
	CreateSprite("Player_Walk_Body_1");
	CreateSprite("Player_Walk_Top_2");
	CreateSprite("Player_Walk_Body_2");
	CreateSprite("Player_Jump_Top_1");
	CreateSprite("Player_Jump_Body_1");
	CreateSprite("Player_Idle_Top_1");
	CreateSprite("Player_Idle_Body_1");
	CreateSprite("Player_Idle_Top_2");
	CreateSprite("Player_Idle_Body_2");
	CreateSprite("Player_Climb_Top_1");
	CreateSprite("Player_Climb_Body_1");
	CreateSprite("Player_Climb_Top_2");
	CreateSprite("Player_Climb_Body_2");
	CreateSprite("Player_Power_Up_Dead_Top");
	CreateSprite("Player_Power_Up_Dead_Body");
	CreateSprite("Player_Power_Up_Walk_Top_1");
	CreateSprite("Player_Power_Up_Walk_Body_1");
	CreateSprite("Player_Power_Up_Walk_Top_2");
	CreateSprite("Player_Power_Up_Walk_Body_2");
	CreateSprite("Player_Power_Up_Jump_Top_1");
	CreateSprite("Player_Power_Up_Jump_Body_1");
	CreateSprite("Player_Power_Up_Idle_Top_1");
	CreateSprite("Player_Power_Up_Idle_Body_1");
	CreateSprite("Player_Power_Up_Idle_Top_2");
	CreateSprite("Player_Power_Up_Idle_Body_2");
	CreateSprite("Player_Power_Up_Climb_Top_1");
	CreateSprite("Player_Power_Up_Climb_Body_1");
	CreateSprite("Player_Power_Up_Climb_Top_2");
	CreateSprite("Player_Power_Up_Climb_Body_2");
	CreateSprite("Fireball_1");
	CreateSprite("Fireball_2");
	CreateSprite("Fireball_3");
	CreateSprite("Fireball_4");
	CreateSprite("Explosion_1");
	CreateSprite("Explosion_2");
	CreateSprite("Explosion_3");
	CreateSprite("Explosion_4");
	CreateSprite("Explosion_5");
	CreateSprite("Explosion_6");
	CreateSprite("Explosion_7");
	CreateSprite("Explosion_8");
	CreateSprite("Grass_Left");
	CreateSprite("Grass_Centre");
	CreateSprite("Grass_Right");
	CreateSprite("Dirt");
	CreateSprite("Grass_Cliff_Left");
	CreateSprite("Grass_Cliff_Right");
	CreateSprite("Stone_Left");
	CreateSprite("Stone_Centre");
	CreateSprite("Stone_Right");
	CreateSprite("Stone_Top");
	CreateSprite("Coin_1");
	CreateSprite("Coin_2");
	CreateSprite("Coin_3");
	CreateSprite("Coin_4");
	CreateSprite("Coin_5");
	CreateSprite("Coin_6");
	CreateSprite("Coin_7");
	CreateSprite("Coin_8");
	CreateSprite("Gem_Fire_1");
	CreateSprite("Gem_Fire_2");
	CreateSprite("Gem_Fire_3");
	CreateSprite("Gem_Fire_4");
	CreateSprite("Gem_Fire_5");
	CreateSprite("Gem_Fire_6");
	CreateSprite("Gem_Grow_1");
	CreateSprite("Gem_Grow_2");
	CreateSprite("Gem_Grow_3");
	CreateSprite("Gem_Grow_4");
	CreateSprite("Gem_Grow_5");
	CreateSprite("Gem_Grow_6");
	CreateSprite("Flag_Up_1");
	CreateSprite("Flag_Up_2");
	CreateSprite("Flag_Down");
	CreateSprite("Flag_Pole");
	CreateSprite("UI_0");
	CreateSprite("UI_1");
	CreateSprite("UI_2");
	CreateSprite("UI_3");
	CreateSprite("UI_4");
	CreateSprite("UI_5");
	CreateSprite("UI_6");
	CreateSprite("UI_7");
	CreateSprite("UI_8");
	CreateSprite("UI_9");
	CreateSprite("UI_C");
	CreateSprite("UI_D");
	CreateSprite("UI_E");
	CreateSprite("UI_I");
	CreateSprite("UI_L");
	CreateSprite("UI_R");
	CreateSprite("UI_S");
	CreateSprite("UI_T");
	CreateSprite("UI_V");
	CreateSprite("UI_W");
	CreateSprite("UI_Heart_Full");
	CreateSprite("UI_Heart_Half");
	CreateSprite("UI_Heart_Empty");
	CreateSprite("UI_Lives");
	CreateSprite("UI_Coins");
	CreateSprite("UI_X");
	CreateSprite("Slime_1");
	CreateSprite("Slime_2");
	CreateSprite("Slime_Squashed");
	CreateSprite("Slime_Snail_Shell_Hit");
	CreateSprite("Snail_1");
	CreateSprite("Snail_2");
	CreateSprite("Snail_Snail_Shell_Hit");
	CreateSprite("Snail_Shell");
	CreateSprite("Snail_Shell_Crack_1");
	CreateSprite("Snail_Shell_Crack_2");
	CreateSprite("Snail_Shell_Crack_3");
	CreateSprite("Block_Item");
	CreateSprite("Block_Boxed_Coin");
	CreateSprite("Block_Coin");
	CreateSprite("Block_Brick");
	CreateSprite("Block_Crate");
	CreateSprite("Ladder_Top");
	CreateSprite("Ladder_Mid");
	CreateSprite("Arrow_Sign");
	CreateSprite("Bush");
	CreateSprite("Plant");
	CreateSprite("Spikes");
	CreateSprite("Rock");
	CreateSprite("Mushroom1");
	CreateSprite("Mushroom2");
	CreateSprite("Door_Closed_Top");
	CreateSprite("Door_Closed_Mid");
	CreateSprite("Door_Open_Top");
	CreateSprite("Door_Open_Mid");
}

void Graphics::ClearScreen(HAPISPACE::HAPI_TColour col) const
{
	for (int i = 0; i < constants::k_screenWidth * constants::k_screenHeight; i++)
	{
		memcpy(m_screen + 4 * i, &col, 4);
	}
}

void Graphics::ClearScreen() const
{
	memset(m_screen, 0, static_cast<size_t>(constants::k_screenWidth * constants::k_screenHeight * 4));
}

void Graphics::SetPixel(const int x, const int y, const HAPISPACE::HAPI_TColour colour) const
{
	m_screen[constants::k_screenWidth * x + y] = colour.red;
	m_screen[constants::k_screenWidth * x + y + 1] = colour.green;
	m_screen[constants::k_screenWidth * x + y + 2] = colour.blue;
	m_screen[constants::k_screenWidth * x + y + 3] = colour.alpha;
}

void Graphics::SetPixel(const int x, const int y, const int value) const
{
	m_screen[constants::k_screenWidth * x + y] = value;
}

bool Graphics::CreateTexture(const std::string& filename, const std::string& name)
{
	auto* newTexture = new Texture();
	if (!newTexture->Initialise(filename))
	{
		HAPI.UserMessage("Could not load the file: " + filename + "\nPlease check the spelling and try again", "Error Occured");
		delete newTexture;
		return false;
	}
	m_textureBuffer[name] = newTexture;
	return true;
}

bool Graphics::CreateSpriteSheet(const std::string& filename)
{
	auto* newSprite = new Texture();
	if (!newSprite->Initialise(filename))
	{
		HAPI.UserMessage("Could not load the file: " + filename + "\nPlease check the spelling and try again", "Error Occured");
		delete newSprite;
		return false;
	} else
	{
		m_spriteSheet = newSprite;
	}
	return true;
}

bool Graphics::CreateSprite(const std::string& spriteName, const int spriteLocation)
{
	if (!(m_spriteSheetLocations.find(spriteName) == m_spriteSheetLocations.end()))
	{
		HAPI.UserMessage("A sprite with the name " + spriteName + " already exists.", "Error Occured");
		return false;
	} else
	{
		m_spriteSheetLocations[spriteName] = spriteLocation;
	}
	return true;
}

bool Graphics::CreateSprite(const std::string& spriteName)
{
	if (!(m_spriteSheetLocations.find(spriteName) == m_spriteSheetLocations.end()))
	{
		HAPI.UserMessage("A sprite with the name " + spriteName + " already exists.", "Error Occured");
		return false;
	} else
	{
		m_spriteSheetLocations[spriteName] = m_nextSpritesheetLocation++;
	}
	return true;
}

void Graphics::DrawTexture(const std::string& name, const Vector2 position)
{
	if (!m_textureBuffer.at(name))
	{
		HAPI.UserMessage("Error: Can't draw the Texture " + name + "\nCheck the Spelling and try again.", "Error :(");
		return;
	}
	m_textureBuffer.at(name)->RenderTexture(m_screen, position);
}

void Graphics::DrawSprite(const std::string& name, const Vector2 position, const bool flipped, const short alpha)
{
	if (m_spriteSheetLocations.find(name) == m_spriteSheetLocations.end())
	{
		HAPI.UserMessage("Error: Can't draw the sprite: " + name + "\nCheck the Spelling and try again.", "Error Occured");
	} else
	{
		m_spriteSheet->RenderSprite(m_screen, m_spriteSheetLocations[name], position, flipped, alpha);
	}
}
