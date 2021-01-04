#include "GameOver.h"
#include <fstream>
#include "../Audio/SoundManager.h"
#include "../Graphics/TextureManager.h"
#include "../State System/StateManager.h"

GameOver::GameOver() :
	State(),
	m_scoreText("123456",
	            {
		            Vector2::CENTRE.x - constants::k_spriteSheetCellSize * 3, constants::k_spriteSheetCellSize * 4
	            }),
	m_highScoreText("HI SCORE",
	                {
		                Vector2::CENTRE.x - constants::k_spriteSheetCellSize * 3.5f,
		                m_scoreText.GetPosition().y + constants::k_spriteSheetCellSize * 2
	                }),
	m_timeElapsedText("199",
	                  {
		                  Vector2::CENTRE.x - constants::k_spriteSheetCellSize * 2,
		                  m_highScoreText.GetPosition().y + constants::k_spriteSheetCellSize * 2
	                  }),
	m_mainMenuText("MAIN MENU",
	               {
		               Vector2::CENTRE.x - 2 * constants::k_spriteSheetCellSize,
		               constants::k_screenHeight - 2 * constants::k_spriteSheetCellSize
	               }),
	m_playerWon(false) {}

bool GameOver::Initialise(TextureManager& textureManager)
{
	std::ifstream readFile("Res/score.txt");
	if (!readFile.is_open())
	{
		return false;
	}

	std::string score, highScore, timeElapsed, playerWon;

	std::getline(readFile, score);
	std::getline(readFile, highScore);
	std::getline(readFile, timeElapsed);
	std::getline(readFile, playerWon);

	m_scoreText.SetString("Score " + score);
	m_highScoreText.SetString("HiScore " + highScore);
	m_timeElapsedText.SetString("Time " + timeElapsed);

	m_playerWon = playerWon == "WIN" ? true : false;

	return textureManager.CreateTexture(m_playerWon
		                                    ? "Res/Graphics/GameOver_Background_Win.tga"
		                                    : "Res/Graphics/GameOver_Background_Lose.tga", "Background");
}

bool GameOver::Unload(TextureManager& textureManager)
{
	return textureManager.RemoveTexture("Background");
}

void GameOver::Input()
{
	const HAPISPACE::HAPI_TControllerData& controllerData = HAPI.GetControllerData(0);

	if (GetKey(eKeyCode::SPACE) || GetKey(eKeyCode::ENTER) || controllerData.digitalButtons[HK_DIGITAL_A])
	{
		STATE_MANAGER.ChangeState(eState::e_MainMenu);
	}
}

void GameOver::Update()
{
	SoundManager::GetInstance().UpdateMusicBufferStream();
}

void GameOver::Render(TextureManager& textureManager)
{
	textureManager.DrawTexture("Background", {});
	m_scoreText.Render(textureManager);
	m_highScoreText.Render(textureManager);
	m_timeElapsedText.Render(textureManager);
	m_mainMenuText.Render(textureManager);
	textureManager.DrawSprite("UI_Pointer", {
		                          m_mainMenuText.GetPosition().x + 5 * constants::k_spriteSheetCellSize,
		                          m_mainMenuText.GetPosition().y
	                          });
}
