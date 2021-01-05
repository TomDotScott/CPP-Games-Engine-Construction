#pragma once
#include "../State System/State.h"
#include "Text.h"

/**
 * \brief The selected button of the main menu
 */
enum class eSelected
{
	e_Play,
	e_Controls
};

/**
 * \brief The MainMenu is the first state the user sees,
 * it gives the user the choice to play the game or see the
 * controls menu
 */
class MainMenu final : public State
{
public:
	MainMenu();
	bool Initialise(TextureManager& textureManager) override;
	bool Unload(TextureManager& textureManager) override;
	void Input() override;
	void Update() override;
	void Render(TextureManager& textureManager) override;

private:
	Text      m_highScoreText;
	Text      m_playText;
	Text      m_controlsText;
	eSelected m_selected;
	// A pointer is displayed on screen, depending on the user's
	// selected option
	Vector2   m_selectorPosition;
};
