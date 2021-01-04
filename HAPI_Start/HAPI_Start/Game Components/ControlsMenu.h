#pragma once
#include "../State System/State.h"
#include "Text.h"

class ControlsMenu final : public State
{
public:
	ControlsMenu();

	bool Initialise(TextureManager& textureManager) override;
	bool Unload(TextureManager& textureManager) override;
	void Input() override;
	void Update() override;
	void Render(TextureManager& textureManager) override;

private:
	Text m_backText;
};
