#pragma once
#include "../State System/State.h"
#include "Text.h"

enum class eSelected
{
	e_Play, e_Controls
};

class MainMenu final : public State
{
public:
	MainMenu(const HAPISPACE::HAPI_TKeyboardData& keyboardData, const HAPISPACE::HAPI_TControllerData& controllerData);
	bool Initialise(TextureManager& textureManager) override;
	void Input() override;
	void Update() override;
	void Render(TextureManager& textureManager) override;
	
private:
	Text m_highScoreText;
	Text m_playText;
	Text m_controlsText;
	
	eSelected m_selected;
	
	Vector2 m_selectorPosition;
};

