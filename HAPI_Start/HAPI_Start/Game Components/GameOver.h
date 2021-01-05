#pragma once
#include "../State System/State.h"
#include "Text.h"

/**
 * \brief The state that gets displayed when the player has finished
 * playing the game demo
 */
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
	// The texture that gets displayed differs depending on
	// whether the player passed the final level or lost all
	// of their lives
	bool m_playerWon;
};
