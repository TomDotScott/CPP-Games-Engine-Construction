#include "MainMenu.h"

#include "../Audio/SoundManager.h"
#include "../Graphics/TextureManager.h"
#include "../State System/StateManager.h"

MainMenu::MainMenu(const HAPISPACE::HAPI_TKeyboardData& keyboardData, const HAPISPACE::HAPI_TControllerData& controllerData) :
	State(keyboardData, controllerData),
	m_highScoreText("HISCORE 123456", { Vector2::CENTRE.x - 224, 64 }),
	m_playText("PLAY", { Vector2::CENTRE.x - constants::k_spriteSheetCellSize, Vector2::CENTRE.y + 2 * constants::k_spriteSheetCellSize }),
	m_controlsText("CONTROLS", { Vector2::CENTRE.x - 2 * constants::k_spriteSheetCellSize, m_playText.GetPosition().y + constants::k_spriteSheetCellSize }),
	m_selected(eSelected::e_Play)
{
}

bool MainMenu::Initialise(TextureManager& textureManager)
{
	SoundManager::GetInstance().AddMusic("MainMenu", "Res/Music/MainMenu.wav");
	SoundManager::GetInstance().PlayMusic("MainMenu");

	return textureManager.CreateTexture("Res/Graphics/MainMenu_Background.tga", "MainMenu_Background");
}

void MainMenu::Input()
{
	if (GetKey(eKeyCode::W))
	{
		m_selected = eSelected::e_Play;
	}

	if (GetKey(eKeyCode::S))
	{
		m_selected = eSelected::e_Controls;
	}

	if (GetKey(eKeyCode::SPACE))
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

	Vector2 pointerPosition{ Vector2::CENTRE.x + 2 * constants::k_spriteSheetCellSize };
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