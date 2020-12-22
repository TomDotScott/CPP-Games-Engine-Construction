#pragma once
#include "State.h"

class TextureManager;

enum class eState
{
	e_MainMenu, e_ControlsMenu, e_LevelSelectionMenu, e_Game, e_GameOver
};

class StateManager
{
public:
	StateManager() = default;

	~StateManager();

	void ChangeState(const eState state);

	void OnCreate(const eState state);

	void Input() const;

	void Update() const;

	void Render(TextureManager& textureManager) const;

	eState GetCurrentState() const { return m_currentState; }
private:
	void SetState(State* state);

	eState m_currentState;
	State* m_state;
};