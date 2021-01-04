#pragma once
#include "../State System/State.h"
#include "Text.h"

class GameOver final : public State
{
public:
	GameOver();
	bool Initialise(TextureManager& textureManager) override;
	bool Unload(TextureManager& textureManager) override;
	void Input() override;
	void Update() override;
	void Render(TextureManager& textureManager) override;

private:
	Text m_scoreText;
	Text m_highScoreText;
	Text m_timeElapsedText;
	Text m_mainMenuText;
	bool m_playerWon;
};
