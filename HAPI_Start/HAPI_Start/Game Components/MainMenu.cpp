#include "MainMenu.h"

#include "../Audio/SoundManager.h"
#include "../Graphics/TextureManager.h"
#include "../State System/StateManager.h"
#include <fstream>

MainMenu::MainMenu() :
	State(),
	m_highScoreText("HISCORE 123456", {Vector2::CENTRE.x - 224, 64}),
	m_playText("PLAY", {
		           Vector2::CENTRE.x - constants::k_spriteSheetCellSize,
		           Vector2::CENTRE.y + 2 * constants::k_spriteSheetCellSize
	           }),
	m_controlsText("CONTROLS", {
		               Vector2::CENTRE.x - 2 * constants::k_spriteSheetCellSize,
		               m_playText.GetPosition().y + constants::k_spriteSheetCellSize
	               }),
	m_selected(eSelected::e_Play) {}

bool MainMenu::Initialise(TextureManager& textureManager)
{
	std::ifstream read("Res/score.txt");

	std::string highscore;
	// Highscore is on the second line, but we have to read both lines
	std::getline(read, highscore);
	std::getline(read, highscore);

	m_highScoreText.SetString("HISCORE " + highscore);

	read.close();

	if (!SoundManager::GetInstance().AddMusic("MainMenu", "Res/Music/MainMenu.wav"))
	{
		return false;
	}

	SoundManager::GetInstance().PlayMusic("MainMenu");

	return textureManager.CreateTexture("Res/Graphics/MainMenu_Background.tga", "MainMenu_Background");
}

bool MainMenu::Unload(TextureManager& textureManager)
{
	return SoundManager::GetInstance().RemoveMusic("MainMenu") && textureManager.RemoveTexture("MainMenu_Background");
}

void MainMenu::Input()
{
	const HAPISPACE::HAPI_TControllerData& controllerData = HAPI.GetControllerData(0);
	
	if (GetKey(eKeyCode::W) || controllerData.digitalButtons[HK_DIGITAL_DPAD_UP])
	{
		m_selected = eSelected::e_Play;
	}

	if (GetKey(eKeyCode::S) || controllerData.digitalButtons[HK_DIGITAL_DPAD_DOWN])
	{
		m_selected = eSelected::e_Controls;
	}

	if (GetKey(eKeyCode::SPACE) || GetKey(eKeyCode::ENTER) || controllerData.digitalButtons[HK_DIGITAL_A])
	{
		switch (m_selected)
		{
			case eSelected::e_Play:
				STATE_MANAGER.ChangeState(eState::e_Game);
				break;
			case eSelected::e_Controls:
				STATE_MANAGER.ChangeState(eState::e_ControlsMenu);
				break;
		}
	}
}

void MainMenu::Update()
{
	SoundManager::GetInstance().UpdateMusicBufferStream();
}

void MainMenu::Render(TextureManager& textureManager)
{
	textureManager.DrawTexture("MainMenu_Background", {});

	m_highScoreText.Render(textureManager);
	m_playText.Render(textureManager);
	m_controlsText.Render(textureManager);

	Vector2 pointerPosition{Vector2::CENTRE.x + 2 * constants::k_spriteSheetCellSize};
	switch (m_selected)
	{
		case eSelected::e_Play:
			pointerPosition.y = m_playText.GetPosition().y;
			break;
		case eSelected::e_Controls:
			pointerPosition.y = m_controlsText.GetPosition().y;
			break;
	}

	textureManager.DrawSprite("UI_Pointer", pointerPosition);
}
