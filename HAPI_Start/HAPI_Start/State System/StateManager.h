#pragma once
#include "../Graphics/TextureManager.h"
#include "HAPI_Types.h"
#include "State.h"


enum class eState
{
	e_MainMenu, e_ControlsMenu, e_LevelSelectionMenu, e_Game, e_GameOver
};

class StateManager
{
public:
	explicit StateManager();

	~StateManager();

	void ChangeState(const eState state);

	void OnCreate(const eState state);

	void Update() const;

	void Render();

	eState GetCurrentState() const { return m_currentState; }
private:
	void SetState(State* state);

	eState m_currentState;
	State* m_state;

	TextureManager m_textureManager;
};

extern StateManager STATE_MANAGER;